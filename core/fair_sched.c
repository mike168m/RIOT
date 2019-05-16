/*
 * Copyright (C) 2019 Michael Osei
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     core_util
 *
 * @{
 * @file
 * @brief       Optional Fair scheduler from the Zircon kernel
 *
 * @author      Michael Osei <mosei27@live.com>
 * 
 * This file contains the function prototypes for an optional Fair scheduler
 * implemented for the RIOT Kernel.
 * 
 * ## Background:

  In fair scheduling the scheduler is copied to each core for execution.
  Each core has a set of weighted threads that are either in RUNNING, PENDING or STOPPED
  states. At most one thread is running on each core. All other threads wait to
  be executed and are in the PENDING state. Threads in the PENDING state are 
  enqueued for execution.

  A fair scehduler uses an ordering technique based on thread weights
  to determine which thread gets executed next. In this implementation of the 
  scheduler queueing is implement with a balanced binary tree.

  ## How threads are ordered:

  Threads are ordered based on 3 factors.

  Virtualized Time: This is the amount of time a thread would take to run 
  on a normalized time slice.

  Normalized Time Slice:  A normalized time slice is the amount of time allocated
  to a thread given its normalized rate and is proportional to it.

  Normalized Rate: The normalized rate of a thread is inversely proportional
  to its weight.

  Because of this if a thread has a higher weight it gets inserted in front 
  of the queue.
 * 
 */
#ifndef FAIR_SCHED_H
#define FAIR_SCHED_H

#include "kernel_defines.h"
#include "kernel_types.h"
#include "native_sched.h"
#include "clist.h"

typedef enum {
    STATUS_STOPPED,                 /**< has terminated                       */
    STATUS_SLEEPING,                /**< sleeping                             */
    STATUS_MUTEX_BLOCKED,           /**< waiting for a locked mutex           */
    STATUS_RECEIVE_BLOCKED,         /**< waiting for a message                */
    STATUS_SEND_BLOCKED,            /**< waiting for message to be delivered  */
    STATUS_REPLY_BLOCKED,           /**< waiting for a message response       */
    STATUS_FLAG_BLOCKED_ANY,        /**< waiting for any flag from flag_mask  */
    STATUS_FLAG_BLOCKED_ALL,        /**< waiting for all flags in flag_mask   */
    STATUS_MBOX_BLOCKED,            /**< waiting for get/put on mbox          */
    STATUS_COND_BLOCKED,            /**< waiting for a condition variable     */
    STATUS_RUNNING,                 /**< currently running                    */
    STATUS_PENDING,                 /**< waiting to be scheduled to run       */
    STATUS_NUMOF      
} thread_status_t;


typedef struct {
    float weight;                   /**< relative weight of the thread.**/
    uint16_t virtual_start_time;    /**< virtual start time of the thread.**/
    uint16_t virtual_end_time;      /**< virtual finish of the thread.**/
    uint16_t time_slice_sz;         /**< time slice of the thread.**/
} fair_thread_status_t;  
/** @} */

typedef struct { 
    uint8_t thread_cnt;             /**< number of threads that can run on a core*/
    uint16_t sched_period;
    float total_weight;
} fair_cpu_status_t;

typedef struct {
    uint16_t granularity;
    uint16_t latency;
} fair_tunable_state;

#endif
