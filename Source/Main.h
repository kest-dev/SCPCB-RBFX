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

// Define entry point.
URHO3D_DEFINE_APPLICATION_MAIN(SCPCB);