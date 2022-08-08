#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

class AbstractModule
{
	public:
		virtual int init() = 0;
		virtual int reset() = 0;
		virtual int exec() = 0;

};

#endif /*ABSTRACTMODULE_H*/
