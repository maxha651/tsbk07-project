//
// Created by Fredrik on 2015-04-07.
//

#include <Component.h>

/**
* A camera class.
*/
class Camera : public Component
{
	
	public:
		Camera();
		~Camera();
		virtual void Update() override;
};
