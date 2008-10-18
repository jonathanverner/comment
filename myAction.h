#ifndef _myAction_H
#define _myAction_H

/***************************************************************
 * myAction.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-18.
 * @Last Change: 2008-10-18.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/


class myAction : public QAction  {
  Q_OBJECT
	private:
		abstractTool *tool;		
	private slots:
		void activate();
	public:
		myAction( const QIcon &icn, const QString text, QObject *parent, abstractTool *tool );
		bool isActionForTool( abstractTool *Tool ) { return ( tool == Tool ); };
	signals:
		void triggered( abstractTool *tool );
};



#endif /* _myAction_H */
