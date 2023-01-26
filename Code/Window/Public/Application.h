#pragma once

class Application
{
	Application(const Application&);
	Application& operator=(const Application&);
public:
	Application() { }
	virtual ~Application() { }

	virtual void Initialize() { }
	virtual void Update(float inDeltaTime) { }
	virtual void Render(float inAspectRatio) { }
	virtual void Shutdown() { }
};