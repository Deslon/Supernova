#ifndef Action_h
#define Action_h

//
// (c) 2018 Eduardo Doria.
//

#include "util/FunctionCallback.h"

namespace Supernova{

    class Object;

    class Action{

        friend class Object;
        friend class Animation;
        
    protected:
        
        float timecount;
        
        bool running;
        Object* object;
        
    public:
        Action();
        virtual ~Action();

        FunctionCallback<void(Object*)> onStart;
        FunctionCallback<void(Object*)> onRun;
        FunctionCallback<void(Object*)> onPause;
        FunctionCallback<void(Object*)> onStop;
        FunctionCallback<void(Object*)> onFinish;
        FunctionCallback<void(Object*,float)> onUpdate;

        Object* getObject();

        void setTimecount(float timecount);

        bool isRunning();

        virtual bool run();
        virtual bool pause();
        virtual bool stop();

        virtual bool update(float interval);
    };
}

#endif /* Action_h */
