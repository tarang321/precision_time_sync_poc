/**
 * @file app.c
 * @brief Phase 3 BG24: bind trigger seq to R[N], then match follow-up M[fu] to stored R[fu].
 *
 * Capture time is still the PRS/TIMER edge. The PTS3 line only names the edge
 * and carries M for the previous trigger.
 */
#include "app.h"
#include "pts_prs_timer_capture.h"
#include "pts_host_rx.h"
#include "pts_network_time.h"

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
  pts_host_rx_init();
  pts_network_time_init();
  printf("PTS Phase4 BG24: logical network time from M[N] R[N]\n");
}

void app_process_action(void)
{
  pts_host_id_t id;
  pts_capture_sample_t cap;

  while (pts_host_rx_pop(&id)) {
    if (!id_hold_push(&id)) {
      s_orphan_ids++;
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
        printf("M[N]<->R[N] seq=%lu M=%lu R=%lu fit=%d net=%lu q=%d rej=%lu\n",
               (unsigned long)id.fu_seq,
               (unsigned long)id.m_ticks,
               (unsigned long)r_fu,
               fit,
               (unsigned long)net,
               (int)pts_network_time_quality(),
               (unsigned long)pts_network_time_rejects());
      } else {
        s_miss_r++;
        printf("follow-up seq=%lu M=%lu missing R (miss=%lu)\n",
               (unsigned long)id.fu_seq,
               (unsigned long)id.m_ticks,
               (unsigned long)s_miss_r);
      }
    }
  }
  pts_network_time_poll();
}
