#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, mleft, mright, mdown, mup;

	Scene::Transform *rat = nullptr;

	float ratspeed = 40.0f;



	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;


	float thevoice_clock = 2;      //just in case the voice needs a time reference?
	int thevoice_fulfilled = 1;  //whether the voice above is ready to give next order

	int startlocation = 0;


	//music coming from the tip of the leg (as a demonstration):
	//std::shared_ptr< Sound::PlayingSample > bg_music;
	std::shared_ptr< Sound::PlayingSample > cabbage_command;
	std::shared_ptr< Sound::PlayingSample > carrot_command;
	std::shared_ptr< Sound::PlayingSample > nice_command;
	int command;

	//camera:
	Scene::Camera *camera = nullptr;

};
