/**
 * @file pts_r_table.h
 * @brief Small seq -> R[N] table for follow-up matching (Si917-only).
 */
#ifndef PTS_R_TABLE_H
#define PTS_R_TABLE_H

#include <stdint.h>

#ifndef PTS_R_TABLE_DEPTH
#define PTS_R_TABLE_DEPTH 64u
#endif

typedef struct {
  uint32_t seq;
  uint32_t r_ticks;
  uint8_t valid;
} pts_r_entry_t;

typedef struct {
  pts_r_entry_t e[PTS_R_TABLE_DEPTH];
  uint16_t next;
} pts_r_table_t;

static inline void pts_r_table_init(pts_r_table_t *t)
{
  uint16_t i;
  t->next = 0u;
  for (i = 0u; i < PTS_R_TABLE_DEPTH; i++) {
    t->e[i].valid = 0u;
  }
}

static inline void pts_r_table_store(pts_r_table_t *t, uint32_t seq, uint32_t r_ticks)
{
  uint16_t i;
  for (i = 0u; i < PTS_R_TABLE_DEPTH; i++) {
    if (t->e[i].valid && (t->e[i].seq == seq)) {
      /* First sample wins — do not redefine R[N]. */
      (void)r_ticks;
      return;
    }
  }
  t->e[t->next].seq     = seq;
  t->e[t->next].r_ticks = r_ticks;
  t->e[t->next].valid   = 1u;
  t->next               = (uint16_t)((t->next + 1u) % PTS_R_TABLE_DEPTH);
}

/** @return 0 ok, -1 miss. */
static inline int pts_r_table_lookup(const pts_r_table_t *t, uint32_t seq, uint32_t *r_ticks)
{
  uint16_t i;
  for (i = 0u; i < PTS_R_TABLE_DEPTH; i++) {
    if (t->e[i].valid && (t->e[i].seq == seq)) {
      *r_ticks = t->e[i].r_ticks;
      return 0;
    }
  }
  return -1;
}

#endif /* PTS_R_TABLE_H */
