#pragma once

#include <Urho3D/Engine/StateManager.h>

using namespace Urho3D;

class SCPCB : public Application
{
    // Enable type information.
    URHO3D_OBJECT(SCPCB, Application);

public:
    /// Construct.
    explicit SCPCB(Context* context);

    /// Setup before engine initialization. Modifies the engine parameters.
    void Setup() override;
    /// Setup after engine initialization. Creates the logo, console & debug HUD.
    void Start() override;
    /// Cleanup after the main loop. Called by Application.
    void Stop() override;

private:
    /// Update event handler.
    void Update(VariantMap& eventData);

    /// Scene to be rendered.
    SharedPtr<Scene> scene_;
    /// Viewport that renders the scene.
    SharedPtr<Viewport> viewport_;

    /// Geometry in the scene.
    WeakPtr<Node> geometryNode_;
};


class GameState : public ApplicationState
{
    // Enable type information.
    URHO3D_OBJECT(GameState, ApplicationState);

public:
    /// Construct.
    explicit GameState(Context* context);

protected:
    /// Activate game state. Executed by StateManager.
    void Activate(StringVariantMap& bundle) override;

    /// Scene.
    SharedPtr<Scene> scene_;

    /// Executes when the application state starts.
    virtual void Start() {}

    /// Is mouse enabled?
    bool mouseEnabled_;
};