//
// (c) 2019 Eduardo Doria.
//

#ifndef ANIMATION_H
#define ANIMATION_H

#include "TimeAction.h"
#include <map>

namespace Supernova {

    class Animation: public Action {

        struct ActionFrame{
            float startTime;
            float endTime;
            Action* action;
        };

    private:

        std::vector<ActionFrame> actions;
        bool ownedActions;
        bool loop;

        std::string name;

        float startTime;
        float endTime;

        bool checkAllKeyframe();

    public:

        Animation();
        Animation(std::string name, bool loop = false);
        virtual ~Animation();

        bool isLoop();
        void setLoop(bool loop);

        void setStartFrame(int frameIndex);
        void setStartTime(float startTime);
        float getStartTime();

        void setEndFrame(int frameIndex);
        void setEndTime(float endTime);
        float getEndTime();

        void setLimits(float startTime, float endTime);

        bool isOwnedActions() const;
        void setOwnedActions(bool ownedActions);

        const std::string &getName() const;
        void setName(const std::string &name);

        void addActionFrame(float startTime, float endTime, Action* action, Object* object);
        void addActionFrame(float startTime, TimeAction* action, Object* object);
        ActionFrame getActionFrame(unsigned int index);
        void clearActionFrames();

        virtual bool run();
        virtual bool stop();

        virtual bool update(float interval);

    };

}


#endif //ANIMATION_H
