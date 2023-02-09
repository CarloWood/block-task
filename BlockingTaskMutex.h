/**
 * block-task -- AIStatefulTask submodule - blocking lock a AIStatefulTaskMutex.
 *
 * @file
 * @brief Halt a thread until a given AIStatefulTaskMutex could be obtained. Declaration of class BlockingTaskMutex.
 *
 * @Copyright (C) 2023  Carlo Wood.
 *
 * RSA-1024 0x624ACAD5 1997-01-26                    Sign & Encrypt
 * Fingerprint16 = 32 EC A7 B6 AC DB 65 A6  F6 F6 55 DD 1C DC FF 61
 *
 * This file is part of block-task.
 *
 * Block-task is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Block-task is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with block-task.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "statefultask/AIStatefulTask.h"
#include "utils/threading/Gate.h"

namespace task {

// Usage:
//
// auto blocking_task = statefultask::create<BlockingTaskMutex>(DEBUG_ONLY(debug));
// blocking_task->set_mutex(task_mutex);
// blocking_task->lock();       // Will not return until task_mutex was locked.
// /* critical area */
// blocking_task->unlock();     // Optional; destroying the BlockingTaskMutex will unlock the mutex too.
//
class BlockingTaskMutex : public AIStatefulTask
{
 private:
  AIStatefulTaskMutex* m_task_mutex{};
  utils::threading::Gate m_until_locked;

 protected:
  using direct_base_type = AIStatefulTask;

  enum BlockingTaskMutex_state_type {
    BlockingTaskMutex_wait_for_lock = direct_base_type::state_end,
    BlockingTaskMutex_locked
  };

 public:
  static state_type constexpr state_end = BlockingTaskMutex_locked + 1;

  BlockingTaskMutex(CWDEBUG_ONLY(bool debug = false)) : AIStatefulTask(CWDEBUG_ONLY(debug))
  {
    DoutEntering(dc::statefultask(mSMDebug), "BlockingTaskMutex() [" << this << "]");
  }

  void set_mutex(AIStatefulTaskMutex& task_mutex)
  {
    m_task_mutex = &task_mutex;
  }

  void lock()
  {
    run();
    m_until_locked.wait();
  }

  void unlock()
  {
    m_task_mutex->unlock();
    m_task_mutex = nullptr;
  }

 protected:
  ~BlockingTaskMutex() override;
  char const* task_name_impl() const override { return "BlockingTaskMutex"; }
  char const* state_str_impl(state_type run_state) const override;
  void multiplex_impl(state_type run_state) override;
  void finish_impl() override;
};

} // namespace task
