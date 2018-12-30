#ifndef ANIMATION_CLIP
#define ANIMATION_CLIP

class AnimationClip {
public:
    enum Mode {
        none,
        clamp,
        loop,
        pong
    };

    Mode mode = none;

    void step(float delta) {

        if (iterations == 0) {
            return;
        }

        time += delta * speed;

        if (time < 0) {
            iterations--;
            switch (mode) {
                case none:
                    time = length;
                    break;
                case clamp:
                    time = 0;
                    break;
                case loop:
                    time = -time;
                    break;
                case pong:
                    speed = -speed;
                    time = -time;
                    break;
            }
        } else if (time > length) {
            iterations--;
            switch (mode) {
                case none:
                    time = 0;
                    break;
                case clamp:
                    time = length;
                    break;
                case loop:
                    time -= length;
                    break;
                case pong:
                    speed = -speed;
                    time = length - (time - length);
                    break;
            }
        }

        act(time);
    }

    float getTime() {
        return time;
    }

    float length = 0;
    float speed = 1;
    int iterations = 1;
private:
    float time = 0;
protected:
    virtual void act(float time) = 0;
};
#endif /* ANIMATION_CLIP */
