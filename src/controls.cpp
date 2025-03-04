/// Include GLFW
#include <glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"
#include "simple_logger.h";
#include "player.h"

extern Player_S monkey;
extern Player_S map;
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;


glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 0.00f, 4.04f, 18.0f ); // should y be 2??
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = -0.270f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;

int maxHieght = 4;

void computeMatricesFromInputs(){
	position = monkey.Ent->model->position;
	position.y += 5;
	position.z += 8;
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	 glfwGetCursorPos(window, &xpos, &ypos);
	// Reset mouse position for next frame
	 glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );
	//slog("%f", verticalAngle);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// tilt forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		//position += direction * deltaTime * speed;
	}
	// tilt backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		//position -= direction * deltaTime * speed;
	}
	// tilt right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		//position += right * deltaTime * speed;
	}
	// tilt left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		//position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	//if(position.y <= maxHieght )
	//{
	//	position.y = maxHieght;
	//}
	
	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;



}