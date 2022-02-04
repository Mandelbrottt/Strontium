#pragma once

// Macro include file.
#include "StrontiumPCH.h"

// STL includes.
#include <mutex>

// Project includes.
#include "Core/ApplicationBase.h"

namespace Strontium
{
  enum class EventType
  {
    KeyPressedEvent, KeyReleasedEvent, KeyTypedEvent, MouseClickEvent,
    MouseReleasedEvent, MouseScrolledEvent, WindowCloseEvent, WindowResizeEvent,
    OpenDialogueEvent, LoadFileEvent, SaveFileEvent, GuiEvent, EntitySwapEvent,
    EntityDeleteEvent
  };

  enum class DialogueEventType
  {
    FileOpen, FileSave, FileSelect
  };

  enum class GuiEventType
  {
    StartSpinnerEvent, EndSpinnerEvent
  };

  // Forward declare classes which need to get shuffled around in events.
  class Scene;

  //----------------------------------------------------------------------------
  // The generic event class which all events inherit from.
  //----------------------------------------------------------------------------
  class Event
  {
  public:
    Event(const EventType &type, const std::string &eventName = "Default event");
    virtual ~Event() = default;

    EventType getType() { return this->type; }
    std::string getName() { return this->name; }

    static void deleteEvent(Event* &event);
  protected:
    EventType type;
    std::string name;
  };

  //----------------------------------------------------------------------------
  // The key pressed event.
  //----------------------------------------------------------------------------
  class KeyPressedEvent : public Event
  {
  public:
    KeyPressedEvent(const int keyCode, const uint repeat);
    ~KeyPressedEvent() = default;

    inline int getKeyCode() { return this->keyCode;}
    inline uint getRepeatCount() { return this->numRepeat; }
  protected:
    int keyCode;
    uint numRepeat;
  };

  //----------------------------------------------------------------------------
  // Key released event.
  //----------------------------------------------------------------------------
  class KeyReleasedEvent : public Event
  {
  public:
    KeyReleasedEvent(const int keyCode);
    ~KeyReleasedEvent() = default;

    inline int getKeyCode() { return this->keyCode;}
  protected:
    int keyCode;
  };

  //----------------------------------------------------------------------------
  // Key typed event.
  //----------------------------------------------------------------------------
  class KeyTypedEvent : public Event
  {
  public:
    KeyTypedEvent(const uint keyCode);
    ~KeyTypedEvent() = default;

    inline int getKeyCode() { return this->keyCode; }
  protected:
    uint keyCode;
  };

  //----------------------------------------------------------------------------
  // Mouse button click event.
  //----------------------------------------------------------------------------
  class MouseClickEvent : public Event
  {
  public:
    MouseClickEvent(const int mouseCode);
    ~MouseClickEvent() = default;

    inline int getButton() const { return this->mouseCode; }
  protected:
    int mouseCode;
  };

  //----------------------------------------------------------------------------
  // Mouse button release event.
  //----------------------------------------------------------------------------
  class MouseReleasedEvent : public Event
  {
  public:
    MouseReleasedEvent(const int mouseCode);
    ~MouseReleasedEvent() = default;

    inline int getButton() const { return this->mouseCode; }
  protected:
    int mouseCode;
  };

  //----------------------------------------------------------------------------
  // Mouse scroll event.
  //----------------------------------------------------------------------------
  class MouseScrolledEvent : public Event
  {
  public:
    MouseScrolledEvent(const float xOffset, const float yOffset);
    ~MouseScrolledEvent() = default;

    inline glm::vec2 getOffset() { return glm::vec2(this->xOffset, this->yOffset); }
  private:
    float xOffset, yOffset;
  };

  //----------------------------------------------------------------------------
  // Window close event (tells the application to stop running).
  //----------------------------------------------------------------------------
  class WindowCloseEvent : public Event
  {
  public:
    WindowCloseEvent();
    ~WindowCloseEvent() = default;
  };

  //----------------------------------------------------------------------------
  // Window resize event.
  //----------------------------------------------------------------------------
  class WindowResizeEvent : public Event
  {
  public:
    WindowResizeEvent(uint width, uint height);
    ~WindowResizeEvent() = default;

    inline glm::ivec2 getSize() { return glm::ivec2(this->width, this->height); }
  private:
    uint width, height;
  };

  //----------------------------------------------------------------------------
  // Open file dialogue event.
  //----------------------------------------------------------------------------
  class OpenDialogueEvent : public Event
  {
  public:
    OpenDialogueEvent(DialogueEventType type = DialogueEventType::FileOpen,
                      const std::string &validFiles = "*.*");
    ~OpenDialogueEvent() = default;

    inline std::string& getFormat() { return this->validFiles; }
    inline DialogueEventType getDialogueType() { return this->dialogueType; }
  private:
    std::string validFiles;
    DialogueEventType dialogueType;
  };

  //----------------------------------------------------------------------------
  // Load file event.
  //----------------------------------------------------------------------------
  class LoadFileEvent : public Event
  {
  public:
    LoadFileEvent(const std::string &absPath, const std::string &fileName);
    ~LoadFileEvent() = default;

    inline std::string& getAbsPath() { return this->absPath; }
    inline std::string& getFileName() { return this->fileName; }
  private:
    std::string absPath;
    std::string fileName;
  };

  class SaveFileEvent : public Event
  {
  public:
    SaveFileEvent(const std::string &absPath, const std::string &fileName);
    ~SaveFileEvent() = default;

    inline std::string& getAbsPath() { return this->absPath; }
    inline std::string& getFileName() { return this->fileName; }
  private:
    std::string absPath;
    std::string fileName;
  };

  //----------------------------------------------------------------------------
  // Gui event.
  //----------------------------------------------------------------------------
  class GuiEvent : public Event
  {
  public:
    GuiEvent(GuiEventType type, const std::string &eventText);
    ~GuiEvent() = default;

    GuiEventType getGuiEventType() { return this->guiEventType; }
    std::string& getText() { return this->eventText; }
  private:
    GuiEventType guiEventType;
    std::string eventText;
  };

  //----------------------------------------------------------------------------
  // The entity swapped event.
  //----------------------------------------------------------------------------
  class EntitySwapEvent : public Event
  {
  public:
    EntitySwapEvent(int entityID, Scene* entityParentScene);
    ~EntitySwapEvent() = default;

    uint getStoredEntity() { return this->storedEntity; }
    Scene* getStoredScene() { return this->entityParentScene; }
  private:
    int storedEntity;
    Scene* entityParentScene;
  };

  //----------------------------------------------------------------------------
  // The entity deleted event.
  //----------------------------------------------------------------------------
  class EntityDeleteEvent : public Event
  {
  public:
    EntityDeleteEvent(int entityID, Scene* entityParentScene);
    ~EntityDeleteEvent() = default;

    uint getStoredEntity() { return this->storedEntity; }
    Scene* getStoredScene() { return this->entityParentScene; }
  private:
    int storedEntity;
    Scene* entityParentScene;
  };

  //----------------------------------------------------------------------------
  // Singleton event reciever and dispatcher.
  // I know there are better ways to write event systems, but this is what makes
  // sense to me as of the time I needed an event system. So, here we are!
  //----------------------------------------------------------------------------
  class EventDispatcher
  {
  public:
    // Destructor.
    ~EventDispatcher();

    // Get the instance of the event system.
    static EventDispatcher* getInstance();

    static std::mutex dispatcherMutex;
    // Queue up an event into the dispatcher for handling at the end of the run
    // loop. This is a first in first out event system. Might do priority events
    // at some point.
    void queueEvent(Event* toAdd);

    // Dequeue an event for handling.
    Event* dequeueEvent();

    inline bool isEmpty() { return this->eventQueue.empty(); }
  private:
    static EventDispatcher* appEvents;

    // The queue of events.
    std::queue<Event*> eventQueue;
  };
}
