#include "Graphics/EditorCamera.h"

// Project includes.
#include "Core/Logs.h"
#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/KeyCodes.h"

namespace Strontium
{
  EditorCamera::EditorCamera(float xCenter, float yCenter, EditorCameraType type)
    : position(0.0f, 0.0f, 1.0f)
    , pivot(0.0f, 1.0f, 0.0f)
    , camFront(glm::normalize(pivot - position))
    , camTop(0.0f, 1.0f, 0.0f)
    , proj(1.0f)
    , lastMouseX(xCenter)
    , lastMouseY(yCenter)
    , yaw(-90.0f)
    , pitch(0.0f)
    , currentType(type)
    , firstClick(true)
  {
    this->view = glm::lookAt(this->position, this->pivot,
                             this->camTop);
  }

  EditorCamera::EditorCamera(float xCenter, float yCenter, const glm::vec3 &initPosition,
                             EditorCameraType type)
    : position(initPosition)
    , pivot(0.0f, 1.0f, 0.0f)
    , camFront(glm::normalize(pivot - position))
    , camTop(0.0f, 1.0f, 0.0f)
    , proj(1.0f)
    , lastMouseX(xCenter)
    , lastMouseY(yCenter)
    , yaw(-90.0f)
    , pitch(0.0f)
    , currentType(type)
    , firstClick(true)
  {
    this->view = glm::lookAt(this->position, this->pivot,
                             this->camTop);
  }

  void
  EditorCamera::init(const float &fov, const float &aspect,
                     const float &near, const float &far)
  {
    this->proj = glm::perspective(glm::radians(fov), aspect, near, far);
    switch (this->currentType)
    {
      case EditorCameraType::Free:
        Application::getInstance()->getWindow()->setCursorCapture(true);
        break;
      case EditorCameraType::Stationary:
        Application::getInstance()->getWindow()->setCursorCapture(false);
        break;
    }

    // Fetch the application window for input polling.
    Shared<Window> appWindow = Application::getInstance()->getWindow();
    glm::vec2 mousePos = appWindow->getCursorPos();
    this->lastMouseX = mousePos.x;
    this->lastMouseY = mousePos.y;

    this->horFOV = fov;
    this->near = near;
    this->far = far;
    this->aspect = aspect;
  }

  // On update function for the camera.
  void
  EditorCamera::onUpdate(float dt, const glm::vec2& viewportSize)
  {
    // Fetch the application window for input polling.
    Shared<Window> appWindow = Application::getInstance()->getWindow();

    glm::vec2 mousePos = appWindow->getCursorPos();

    switch (this->currentType)
    {
      //------------------------------------------------------------------------
      // Arcball camera.
      //------------------------------------------------------------------------
      case EditorCameraType::Stationary:
      {
        //----------------------------------------------------------------------
        // Handles the mouse input component.
        //----------------------------------------------------------------------
        auto right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), this->camFront));
        if (appWindow->isKeyPressed(SR_KEY_LEFT_ALT) && appWindow->isMouseClicked(SR_MOUSE_BUTTON_RIGHT))
        {
          float dAngleX = 2.0f * glm::pi<float>() * (this->lastMouseX - mousePos.x) / viewportSize.x;
          float dAngleY = glm::pi<float>() * (this->lastMouseY - mousePos.y) / viewportSize.y;
          
          float cosTheta = glm::dot(this->camFront, glm::vec3(0.0f, 1.0f, 0.0f));
          if (cosTheta * glm::sign(dAngleY) > 0.99f)
            dAngleY = 0.0f;
          
          auto orientation = glm::quat(glm::vec3(0.0f, 1.0f, 0.0f) + this->camFront * dAngleY, glm::vec3(0.0f, 1.0f, 0.0f));
          orientation *= glm::quat(this->camFront + right * (-dAngleX), this->camFront);
          orientation = glm::normalize(orientation);

          auto rot = glm::toMat4(orientation);
          auto tempPosition = rot * (glm::vec4(this->position, 1.0f) - glm::vec4(this->pivot, 0.0f)) + glm::vec4(this->pivot, 0.0f);

          this->position = glm::vec3(tempPosition);
          this->camFront = glm::normalize(this->pivot - this->position);
        }

        //----------------------------------------------------------------------
        // Handles the keyboard input component.
        //----------------------------------------------------------------------
        if (appWindow->isKeyPressed(SR_KEY_LEFT_ALT) && appWindow->isMouseClicked(SR_MOUSE_BUTTON_MIDDLE))
        {
          float dx = (this->lastMouseX - mousePos.x) / viewportSize.x;
          float dy = (this->lastMouseY - mousePos.y) / viewportSize.y;

          auto frontRelativeUp = glm::normalize(glm::cross(this->camFront, right));

          this->position += right * dx;
          this->position += frontRelativeUp * dy;
          this->pivot += right * dx;
          this->pivot += frontRelativeUp * dy;
        }

        break;
      }

      //------------------------------------------------------------------------
      // Free-form camera.
      //------------------------------------------------------------------------
      case EditorCameraType::Free:
      {
        //----------------------------------------------------------------------
        // Handles the mouse input component.
        //----------------------------------------------------------------------
        float dx = mousePos.x - this->lastMouseX;
        float dy = this->lastMouseY - mousePos.y;

        // Compute the yaw and pitch from mouse position.
        this->yaw += (this->sensitivity * dx);
        this->pitch += (this->sensitivity * dy);

        if (this->pitch > 89.0f)
          this-> pitch = 89.0f;
        if (this->pitch < -89.0f)
          this->pitch = -89.0f;

        // Compute the new front vector.
        float distanceToPivot = glm::length(this->pivot - this->position);
        glm::vec3 temp;
        temp.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        temp.y = sin(glm::radians(this->pitch));
        temp.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->camFront = glm::normalize(temp);
        this->pivot = this->position + (this->camFront * distanceToPivot);

        //----------------------------------------------------------------------
        // Handles the keyboard input component.
        //----------------------------------------------------------------------
        float cameraSpeed = this->scalarSpeed * dt;
        if (appWindow->isKeyPressed(SR_KEY_W))
        {
          auto velocity = this->camFront * cameraSpeed;
          this->position += velocity;
          this->pivot += velocity;
        }

        if (appWindow->isKeyPressed(SR_KEY_S))
        {
          auto velocity = -this->camFront * cameraSpeed;
          this->position += velocity;
          this->pivot += velocity;
        }

        if (appWindow->isKeyPressed(SR_KEY_A))
        {
          auto velocity = -glm::normalize(glm::cross(this->camFront, this->camTop))
                           * cameraSpeed;
          this->position += velocity;
          this->pivot += velocity;
        }

        if (appWindow->isKeyPressed(SR_KEY_D))
        {
          auto velocity = glm::normalize(glm::cross(this->camFront, this->camTop))
                          * cameraSpeed;
          this->position += velocity;
          this->pivot += velocity;
        }

        if (appWindow->isKeyPressed(SR_KEY_SPACE))
        {
          auto velocity = this->camTop * cameraSpeed;
          this->position += velocity;
          this->pivot += velocity;
        }

        if (appWindow->isKeyPressed(SR_KEY_LEFT_CONTROL))
        {
          auto velocity = -this->camTop * cameraSpeed;
          this->position += velocity;
          this->pivot += velocity;
        }

        break;
      }
    }

    this->lastMouseX = mousePos.x;
    this->lastMouseY = mousePos.y;

    // Recompute the view matrix.
    this->view = glm::lookAt(this->position, this->pivot,
                             this->camTop);
  }

  // Camera zoom function.
  void
  EditorCamera::cameraZoom(glm::vec2 offsets)
  {
    float cameraSpeed = 0.02 * (offsets.y) * this->scalarSpeed;

    this->position += this->camFront * cameraSpeed;
    this->camFront = glm::normalize(this->pivot - this->position);

    this->view = glm::lookAt(this->position, this->pivot,
                             this->camTop);
  }

  // The event handling function.
  void
  EditorCamera::onEvent(Event &event)
  {
    switch (event.getType())
    {
      case EventType::MouseScrolledEvent:
        this->onMouseScroll(*(static_cast<MouseScrolledEvent*>(&event)));
        break;
      case EventType::KeyPressedEvent:
        this->onKeyPress(*(static_cast<KeyPressedEvent*>(&event)));
        break;
      case EventType::MouseClickEvent:
        this->onMouseClick(*(static_cast<MouseClickEvent*>(&event)));
        break;
      case EventType::MouseReleasedEvent:
        this->onMouseReleased(*(static_cast<MouseReleasedEvent*>(&event)));
        break;
      default: break;
    }
  }

  void
  EditorCamera::onMouseScroll(MouseScrolledEvent &mouseEvent)
  {
    // Fetch the application window for input polling.
    Shared<Window>appWindow = Application::getInstance()->getWindow();

    glm::vec2 offsets = mouseEvent.getOffset();

    switch (this->currentType)
    {
      case EditorCameraType::Stationary:
        if (offsets.y != 0.0 && appWindow->isKeyPressed(SR_KEY_LEFT_ALT))
          this->cameraZoom(offsets);
        break;
      default: break;
    }
  }

  void
  EditorCamera::onKeyPress(KeyPressedEvent &keyEvent)
  {
    // Fetch the application window for input polling.
    Shared<Window> appWindow = Application::getInstance()->getWindow();

    int keyCode = keyEvent.getKeyCode();

    if (keyCode == SR_KEY_P 
		&& appWindow->isKeyPressed(SR_KEY_LEFT_ALT) 
		&& !appWindow->isMouseClicked(SR_MOUSE_BUTTON_RIGHT))
      this->swap();
  }

  void EditorCamera::onMouseClick(const MouseClickEvent& mouseEvent)
  {
	int mouseButton = mouseEvent.getButton();

  	if (mouseButton == SR_MOUSE_BUTTON_RIGHT && isStationary())
  	  this->swap();
  }

  void EditorCamera::onMouseReleased(const MouseReleasedEvent& mouseEvent)
  {
	int mouseButton = mouseEvent.getButton();

  	if (mouseButton == SR_MOUSE_BUTTON_RIGHT && !isStationary())
  	  this->swap();
  }

  // Swap the camera types.
  void
  EditorCamera::swap()
  {
    Logger* logs = Logger::getInstance();
    Shared<Window> appWindow = Application::getInstance()->getWindow();

    if (this->currentType == EditorCameraType::Stationary)
    {
      this->currentType = EditorCameraType::Free;
      appWindow->setCursorCapture(true);
      logs->logMessage(LogMessage("Swapped camera to free-form.", true, false));
    }
    else
    {
      this->currentType = EditorCameraType::Stationary;
      appWindow->setCursorCapture(false);
      logs->logMessage(LogMessage("Swapped camera to stationary.", true, false));

      glm::vec2 cursorPos = appWindow->getCursorPos();
      this->lastMouseX = cursorPos.x;
      this->lastMouseY = cursorPos.y;
    }
  }

  // Update the projection matrix.
  void
  EditorCamera::updateProj(float fov, float aspect, float near, float far)
  {
    this->proj = glm::perspective(glm::radians(fov), aspect, near, far);

    this->horFOV = fov;
    this->near = near;
    this->far = far;
    this->aspect = aspect;
  }

  // Fetch the view/projection matrix of the camera.
  glm::mat4&
  EditorCamera::getViewMatrix()
  {
    return this->view;
  }

  glm::mat4&
  EditorCamera::getProjMatrix()
  {
    return this->proj;
  }

  // Fetch the camera position (for shading).
  glm::vec3
  EditorCamera::getCamPos()
  {
    return this->position;
  }

  // Fetch the camera front vector (for shading).
  glm::vec3
  EditorCamera::getCamFront()
  {
    return this->camFront;
  }
}
