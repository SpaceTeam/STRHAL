#ifndef ABSTRACTCONTROLINPUTCHANNEL_H
#define ABSTRACTCONTROLINPUTCHANNEL_H

#include <can_houbolt/can_cmds.h>
#include <can_houbolt/cmds.h>

class AbstractControlInputChannel
{
	public:
		AbstractControlInputChannel();
		virtual ~AbstractControlInputChannel();

		virtual uint16_t getMeasurement() const = 0;

	protected:

	private:

};

#endif /*ABSTRACTCONTROLINPUTCHANNEL_H*/
