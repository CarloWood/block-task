#include "sys.h"
#include "BlockingTaskMutex.h"

namespace task {

BlockingTaskMutex::~BlockingTaskMutex()
{
  DoutEntering(dc::statefultask, "BlockingTaskMutex::~BlockingTaskMutex() [" << this << "]");
  if (m_task_mutex)
    unlock();
}

char const* BlockingTaskMutex::state_str_impl(state_type run_state) const
{
  switch(run_state)
  {
    AI_CASE_RETURN(BlockingTaskMutex_wait_for_lock);
    AI_CASE_RETURN(BlockingTaskMutex_locked);
  }
  AI_NEVER_REACHED;
}

void BlockingTaskMutex::multiplex_impl(state_type run_state)
{
  switch (run_state)
  {
    case BlockingTaskMutex_wait_for_lock:
      set_state(BlockingTaskMutex_locked);
      if (!m_task_mutex->lock(this, 1))
      {
        wait(1);
        break;
      }
      [[fallthrough]];
    case BlockingTaskMutex_locked:
    {
      finish();
      break;
    }
  }
}

void BlockingTaskMutex::finish_impl()
{
  m_until_locked.open();
}

} // namespace task
