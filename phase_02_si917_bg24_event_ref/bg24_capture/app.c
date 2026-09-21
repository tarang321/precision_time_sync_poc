/**
 * @file app.c
 * @brief Phase 2 BG24: associate hardware R[] with delayed Si917 seq frames.
 *
 * FIFO match: next host identity binds to oldest unmatched capture.
 * If host frame arrives before capture (rare), identity is buffered until
 * a capture appears.
 */
#include "app.h"
#include "pts_prs_timer_capture.h"
#include "pts_host_rx.h"

#include <stdio.h>

#define PTS_ID_HOLD_SIZE 32u

static pts_host_id_t s_id_hold[PTS_ID_HOLD_SIZE];
static uint32_t s_id_hold_head;
static uint32_t s_id_hold_tail;
static uint32_t s_pair_count;
static uint32_t s_orphan_captures;
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

void app_init(void)
{
  pts_prs_timer_capture_init();
  pts_host_rx_init();
  printf("PTS Phase2 BG24: PRS+TIMER capture + host RX ready\n");
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

  /* Associate: for each pending identity, wait for a capture. */
  while (id_hold_pop(&id)) {
    if (!pts_prs_timer_capture_pop(&cap)) {
      /* Put identity back; capture not yet available. */
      (void)id_hold_push(&id);
      break;
    }
    s_pair_count++;
    printf("R[N] pair seq=%lu board=%u R=%lu irq=%lu RSSI=%d pending_cap=%lu ovf=%lu\n",
           (unsigned long)id.seq,
           (unsigned)id.board_id,
           (unsigned long)cap.r_ticks,
           (unsigned long)cap.irq_seq,
           (int)id.rssi,
           (unsigned long)pts_prs_timer_capture_pending(),
           (unsigned long)pts_prs_timer_capture_overflows());
  }

  /* Optional: drain excess captures with no identity (missed UART). */
  if ((pts_prs_timer_capture_pending() > (PTS_CAPTURE_RING_SIZE / 2u))
      && (s_id_hold_head == s_id_hold_tail)) {
    if (pts_prs_timer_capture_pop(&cap)) {
      s_orphan_captures++;
      if ((s_orphan_captures % 50u) == 1u) {
        printf("orphan capture R=%lu (no host id yet) orphans=%lu\n",
               (unsigned long)cap.r_ticks,
               (unsigned long)s_orphan_captures);
      }
    }
  }

  (void)s_pair_count;
}
