#pragma once

#include "statefultask/AIStatefulTask.h"
#include "utils/threading/Gate.h"

namespace task {

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
