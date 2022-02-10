#ifndef ABSTRACTBOARDUNIT_H
#define ABSTRACTBOARDUNIT_H

class AbstractBoardUnit {
	public:
		AbstractBoardUnit();
		~AbstractBoardUnit();

		virtual int init() = 0;
		virtual int exec() = 0;

	private:

};

#endif /*ABSTRACTBOARDUNIT_H*/
