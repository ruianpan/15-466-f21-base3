#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>

GLuint garden_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > garden_meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("garden.pnct"));
	garden_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > garden_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("garden.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = garden_meshes->lookup(mesh_name);

		scene.drawables.emplace_back(transform);
		Scene::Drawable &drawable = scene.drawables.back();

		drawable.pipeline = lit_color_texture_program_pipeline;

		drawable.pipeline.vao = garden_meshes_for_lit_color_texture_program;
		drawable.pipeline.type = mesh.type;
		drawable.pipeline.start = mesh.start;
		drawable.pipeline.count = mesh.count;

	});
});




Load< Sound::Sample > cabbage_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("cabbage.wav"));
});

Load< Sound::Sample > carrot_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("carrot.wav"));
});

Load< Sound::Sample > nice_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("nice.wav"));
});

PlayMode::PlayMode() : scene(*garden_scene) {

	for (auto &transform : scene.transforms) {
		if (transform.name[0] == 'w') rat = &transform;
	}
	if (rat == nullptr) throw std::runtime_error("rat not found.");



	camera = &scene.cameras.front();
	//bg_music = Sound::loop_3D(*wind_sample, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f), 10.0f);
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_ESCAPE) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		} else if (evt.key.keysym.sym == SDLK_a) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}


		else if (evt.key.keysym.sym == SDLK_LEFT) {
			mleft.downs += 1;
			mleft.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			mright.downs += 1;
			mright.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			mup.downs += 1;
			mup.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			mdown.downs += 1;
			mdown.pressed = true;
			return true;
		}




	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.pressed = false;
			return true;
		}

		else if (evt.key.keysym.sym == SDLK_LEFT) {
			mleft.pressed = false;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_RIGHT) {
			mright.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			mup.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			mdown.pressed = false;
			return true;
		}



	}

	return false;
}

void PlayMode::update(float elapsed) {
	thevoice_clock += elapsed;

	{

		//combine inputs into a move:
		constexpr float PlayerSpeed = 30.0f;
		glm::vec2 move = glm::vec2(0.0f);
		if (left.pressed && !right.pressed) move.x =-1.0f;
		if (!left.pressed && right.pressed) move.x = 1.0f;
		if (down.pressed && !up.pressed) move.y =-1.0f;
		if (!down.pressed && up.pressed) move.y = 1.0f;



		//make it so that moving diagonally doesn't go faster:
		if (move != glm::vec2(0.0f)) move = glm::normalize(move) * PlayerSpeed * elapsed;

		glm::mat4x3 frame = camera->transform->make_local_to_parent();
		glm::vec3 right = frame[0];
		//glm::vec3 up = frame[1];
		glm::vec3 forward = -frame[2];

		camera->transform->position += move.x * right + move.y * forward;
	}

	if(thevoice_fulfilled == 1 && thevoice_clock >=2){
		thevoice_fulfilled = 0;
		command=rand() % 2;
		if(command == 0){
			cabbage_command = Sound::play_3D(*cabbage_sample, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 10.0f);
			startlocation = 0;
			if(rat->position.x >= -74 && rat->position.x <=-64){
				startlocation = 3;
			}
			else if(rat->position.x >= -213 && rat->position.x <=-203){
				startlocation = 4;
			}


	}
		else if(command = 1){
			carrot_command = Sound::play_3D(*carrot_sample, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 10.0f);
			startlocation = 0;
			if(rat->position.x >= -129 && rat->position.x <=-119){
				startlocation = 1;
			}
			else if(rat->position.x >= -278 && rat->position.x <=-268){
				startlocation = 2;
			}
		}
	}

	if(mleft.downs){
		rat->position.x -=elapsed*ratspeed;
	}
	if(mright.downs){
		rat->position.x +=elapsed*ratspeed;
	}
	if(mdown.downs){
		rat->position.y -=elapsed*ratspeed;
	}
	if(mup.downs){
		rat->position.y +=elapsed*ratspeed;
	}


	if(rat->position.x < -300){
		rat->position.x = -300;
	}
	if(rat->position.x > -30){
		rat->position.x = -30;
	}

	if(rat->position.y < -70){
		rat->position.y = -70;
	}
	if(rat->position.y > 130){
		rat->position.y = 130;
	}

	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
	mleft.downs = 0;
	mright.downs = 0;
	mup.downs = 0;
	mdown.downs = 0;

	if(rat->position.y > 110 || rat->position.y < -45 || thevoice_fulfilled == 1){
		return;
	}


	if(command == 0){

		if( (rat->position.x >= -74 && rat->position.x <=-64 && startlocation != 3) || (rat->position.x >= -213 && rat->position.x <=-203 && startlocation != 4)  ){
			thevoice_fulfilled = 1;
			thevoice_clock = 0;
		  nice_command = Sound::play_3D(*nice_sample, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 10.0f);
		}
	}

	if(command == 1){

		if( (rat->position.x >= -129 && rat->position.x <=-119 && startlocation != 1) || (rat->position.x >= -278 && rat->position.x <=-268 && startlocation != 2)  ){
			thevoice_fulfilled = 1;
			thevoice_clock = 0;
			nice_command = Sound::play_3D(*nice_sample, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 10.0f);
		}
}







}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//update camera aspect ratio for drawable:
	camera->aspect = float(drawable_size.x) / float(drawable_size.y);

	//set up light type and position for lit_color_texture_program:
	// TODO: consider using the Light(s) in the scene to do this
	glUseProgram(lit_color_texture_program->program);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUseProgram(0);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	scene.draw(*camera);


	GL_ERRORS();
}
