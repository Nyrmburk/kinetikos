#ifndef ANIMAITON_CLIP
#define ANIMATION_CLIP

class AnimationClip {
public:
    AnimationClip(float length) :
            length(length) {}

    virtual void act(float time) = 0;

    void step(float delta) {
        time += delta;

        if (time > length) {
            time = length; // hold at end of animation
            //time -= length; // loop animation
        }
        act(time);
    }

    float length = 0;
private:
    float time = 0;
};
#endif /* ANIMATION_CLIP */
