#ifndef ABSTRACTCONTROLOUTPUTCHANNEL_H
#define ABSTRACTCONTROLOUTPUTCHANNEL_H

#include <can_houbolt/can_cmds.h>
#include <can_houbolt/cmds.h>

class AbstractControlOutputChannel
{
	public:
		AbstractControlOutputChannel();
		virtual ~AbstractControlOutputChannel();


		virtual uint32_t getState() const = 0;
		virtual int setState(uint32_t state) = 0;
		virtual bool isAnalog() = 0;



	protected:

	private:

};

#endif /*ABSTRACTCONTROLOUTPUTCHANNEL_H*/
