#ifndef _H_APPLICATION_
#define _H_APPLICATION_

class Application {
private:
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
public:
	Application() = default;
	virtual ~Application() = default;

	virtual void Initialize() { }
	virtual void Update(float inDeltaTime) { }
	virtual void Render(float inAspectRatio) { }
	virtual void Shutdown() { }
};

#endif
