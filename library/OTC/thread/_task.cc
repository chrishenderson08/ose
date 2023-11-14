/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     misc/_task.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/thread/task.hh>

#include <OTC/stdlib/iostream.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

class Task : public OTC_Task
{
  public:

			Task(int theCount)
			  : count_(theCount) {}

    int			count() const
    				{ return count_; }

  protected:

    void		execute();

    void		fail();

  private:

    int			count_;
};

void Task::execute()
{
  cout << "execute " << count_ << endl;

  if (isStopping())
  {
    cout << "abort " << count_ << endl;

    abortTask();
  }

  if ((count_ % 2) == 0)
  {
    cout << "except " << count_ << endl;

    throw 1;
  }
}

void Task::fail()
{
  cout << "fail " << count_ << endl;
}

void test1()
{
  Task* theTask;

  cout << "create" << endl;

  theTask = new Task(1);

  cout << "start" << endl;

  theTask->start();

  cout << "wait" << endl;

  theTask->wait();

  cout << "state";
  cout << " " << theTask->count();
  cout << " " << theTask->state();
  cout << endl;

  cout << "delete" << endl;

  delete theTask;

  cout << "exit" << endl;
}

void test2()
{
  int const COUNT = 512;

  Task* theTasks[COUNT];

  cout << "create" << endl;

  int i;

  for (i=0; i<COUNT; i++)
    theTasks[i] = new Task(i+1);

  cout << "start" << endl;

  for (i=0; i<COUNT; i++)
  {
    theTasks[i]->start();

    if (((i+1) % 4) == 0)
      theTasks[i]->stop();
  }

  cout << "wait" << endl;

  for (i=0; i<COUNT; i++)
    theTasks[i]->wait();

  for (i=0; i<COUNT; i++)
  {
    cout << "state";
    cout << " " << theTasks[i]->count();
    cout << " " << theTasks[i]->state();

    if (((i+1) % 4) == 0)
    {
      if (theTasks[i]->state() != OTCLIB_TASK_STATE_STOPPED)
        cout << " XXX";
    }

    cout << endl;
  }

  cout << "delete" << endl;

  for (i=0; i<COUNT; i++)
    delete theTasks[i];

  cout << "exit" << endl;
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2
};

/* ------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
  u_int const numTests = sizeof(tests)/sizeof(tests[0]);

  if (argc != 2)
  {
    cout << numTests << endl;
    return 1;
  }
  else
  {
    int testNum = atoi(argv[1]);
    if (testNum > 0 && u_int(testNum) <= numTests)
    {
      tests[testNum-1]();
      return 0;
    }
    else
      return 1;
  }
}

/* ------------------------------------------------------------------------- */
