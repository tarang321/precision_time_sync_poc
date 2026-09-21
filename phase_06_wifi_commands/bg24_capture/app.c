/**
 * @file app.c
 * @brief Phase 6 BG24: network time + scheduled TIMER0 CC1 GPIO toggle.
 */
#include "app.h"
#include "pts_prs_timer_capture.h"
#include "pts_host_rx.h"
#include "pts_network_time.h"
#include "pts_schedule_gpio.h"

#include <stdio.h>

#define PTS_ID_HOLD_SIZE 32u
#define PTS_R_SLOTS      32u

typedef struct {
  uint32_t seq;
  uint32_t r_ticks;
  uint8_t  used;
} pts_r_slot_t;

static pts_host_id_t s_id_hold[PTS_ID_HOLD_SIZE];
static uint32_t s_id_hold_head;
static uint32_t s_id_hold_tail;
static pts_r_slot_t s_r[PTS_R_SLOTS];
static uint32_t s_r_next;
static uint32_t s_pair_count;
static uint32_t s_miss_r;
static uint32_t s_orphan_ids;

static bool id_hold_push(const pts_host_id_t *id)
{
  uint32_t next = (s_id_hold_head + 1u) % PTS_ID_HOLD_SIZE;
  if (next == s_id_hold_tail) {
    return false;
  }
  s_id_hold[s_id_hold_head] = *id;
  s_id_hold_head = next;
  return true;
}

static bool id_hold_pop(pts_host_id_t *id)
{
  if (s_id_hold_tail == s_id_hold_head) {
    return false;
  }
  *id = s_id_hold[s_id_hold_tail];
  s_id_hold_tail = (s_id_hold_tail + 1u) % PTS_ID_HOLD_SIZE;
  return true;
}

static void r_store(uint32_t seq, uint32_t r_ticks)
{
  for (uint32_t i = 0; i < PTS_R_SLOTS; i++) {
    if (s_r[i].used && (s_r[i].seq == seq)) {
      s_r[i].r_ticks = r_ticks;
      return;
    }
  }
  s_r[s_r_next].seq     = seq;
  s_r[s_r_next].r_ticks = r_ticks;
  s_r[s_r_next].used    = 1u;
  s_r_next = (s_r_next + 1u) % PTS_R_SLOTS;
}

static int r_lookup(uint32_t seq, uint32_t *r_ticks)
{
  for (uint32_t i = 0; i < PTS_R_SLOTS; i++) {
    if (s_r[i].used && (s_r[i].seq == seq)) {
      *r_ticks = s_r[i].r_ticks;
      return 0;
    }
  }
  return -1;
}

void app_init(void)
{
  pts_prs_timer_capture_init();
  pts_schedule_gpio_init();
  pts_host_rx_init();
  pts_network_time_init();
  printf("PTS Phase6 BG24: schedule GPIO at future network time (PC02)\n");
}

void app_process_action(void)
{
  pts_host_id_t id;
  pts_host_sched_t sch;
  pts_capture_sample_t cap;

  while (pts_host_rx_pop(&id)) {
    if (!id_hold_push(&id)) {
      s_orphan_ids++;
    }
  }

  while (pts_host_rx_pop_sched(&sch)) {
    if (sch.status == 0u) {
      (void)pts_schedule_gpio_arm(sch.cmd_id, sch.cmd_type, sch.exec_m);
    }
  }

  /* Do not pop an identity until a capture exists, so order stays FIFO. */
  while ((s_id_hold_tail != s_id_hold_head) && (pts_prs_timer_capture_pending() > 0u)) {
    uint32_t r_fu = 0;
    if (!id_hold_pop(&id) || !pts_prs_timer_capture_pop(&cap)) {
      break;
    }
    r_store(id.trigger_seq, cap.r_ticks);
    pts_network_time_on_session(id.session);
    if (id.fu_valid) {
      if (r_lookup(id.fu_seq, &r_fu) == 0) {
        uint32_t net = 0;
        int fit = pts_network_time_add_pair(id.fu_seq, id.m_ticks, r_fu);
        s_pair_count++;
        (void)pts_network_time_now(&net);
        if ((s_pair_count % 25u) == 0u) {
          printf("M[N]<->R[N] seq=%lu M=%lu R=%lu fit=%d net=%lu q=%d exec=%lu\n",
                 (unsigned long)id.fu_seq,
                 (unsigned long)id.m_ticks,
                 (unsigned long)r_fu,
                 fit,
                 (unsigned long)net,
                 (int)pts_network_time_quality(),
                 (unsigned long)pts_schedule_gpio_exec_count());
        }
      } else {
        s_miss_r++;
      }
    }
  }
  pts_network_time_poll();
}
