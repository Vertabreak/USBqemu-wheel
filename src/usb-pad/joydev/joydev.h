#pragma once
#include <linux/joystick.h>
#include <unistd.h>
#include "linux/util.h"
#include "../evdev/evdev-ff.h"
#include "../evdev/shared.h"

namespace usb_pad { namespace joydev {

class JoyDevPad : public Pad
{
public:
	JoyDevPad(int port): Pad(port)
	, mIsGamepad(false)
	, mIsDualAnalog(false)
	, mEvdevFF(nullptr)
	{
	}

	~JoyDevPad() { Close(); }
	int Open();
	int Close();
	int TokenIn(uint8_t *buf, int len);
	int TokenOut(const uint8_t *data, int len);
	int Reset() { return 0; }

	static const TCHAR* Name()
	{
		return "Joydev";
	}

	static int Configure(int port, void *data);
protected:
	int mHandle;
	int mHandleFF;
	evdev::EvdevFF *mEvdevFF;
	struct wheel_data_t mWheelData;
	uint8_t mAxisMap[ABS_MAX + 1];
	bool mAxisInverted[3];
	uint16_t mBtnMap[KEY_MAX + 1];
	int mAxisCount;
	int mButtonCount;

	std::vector<uint16_t> mMappings;

	bool mIsGamepad; //xboxish gamepad
	bool mIsDualAnalog; // tricky, have to read the AXIS_RZ somehow and
						// determine if its unpressed value is zero
};

template< size_t _Size >
bool GetJoystickName(const std::string& path, char (&name)[_Size])
{
	int fd = 0;
	if ((fd = open(path.c_str(), O_RDONLY)) < 0)
	{
		fprintf(stderr, "Cannot open %s\n", path.c_str());
	}
	else
	{
		if (ioctl(fd, JSIOCGNAME(_Size), name) < -1)
		{
			fprintf(stderr, "Cannot get controller's name\n");
			close(fd);
			return false;
		}
		close(fd);
		return true;
	}
	return false;
}

}} //namespace