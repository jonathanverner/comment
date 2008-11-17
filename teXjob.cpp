/***************************************************************
 * teXjob.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-11-17.
 * @Last Change: 2008-11-17.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/

#include <QtCore/QDebuq>
#include <QtCore/QProcess>

#include <poppler/poppler-qt4.h>

#include "teXjob.h"


QString compileJob::latexPath("/usr/bin/pdfLaTeX");
QString compileJob::gsPath("/usr/bin/gs");
bool compileJob::paths_ok = true;

compileJob::compileJob():
	proc(NULL), jobStarted(false)
{
  proc = new QProcess( this );
}

compileJob::~compileJob() { 
  if ( jobStarted || (proc->state() != QProcess::NotRunning)) {
    proc->kill();
  }
  delete proc;
}

void compileJob::setPaths( QString latex, QString gs ) { 
  if ( checkPaths( latex, gs ) ) { 
    latexPath = latex;
    gsPath = gs;
    paths_ok = true;
  }
}

bool compileJob::pathsOK() { 
  return paths_ok;
}

bool compileJob::checkPaths( QString latex, QString gs ) { 
  return true;
}

void compileJob::start( QString latexSource ) { 
  if ( jobStarted ) { 
    qWarning() << "Cannot start while another job in progress. Please use the restart method";
    return;
  }
  if ( ! tmpSRC.open() ) {
    qWarning() << "Cannot open temporary file.";
    emit finished( "", false );
  }
  tmpSRC.write(latexSource.toLocal8Bit()); //FIXME: can fail if unexpected characters
  tmpSRC.flush();
  jobStarted=true;
  disconnect( proc, 0, 0, 0 );
  connect( proc, SIGNAL( finished(int,QProcess::ExitStatus) ), this, texJobFinished(int,QProcess::ExitStatus) ); 
  proc->start( latexPath , tmpSRC.fileName() );
}

void compileJob::restart( QString latexSource ) { 
  disconnect( proc, 0, 0, 0 );
  if ( jobStarted ||  (proc->state() != QProcess::NotRunning) ) { 
    proc->kill();
    removeTempFiles();
    jobStarted=false;
  }
  start( latexSource );
}

void compileJob::kill() { 
   disconnect( proc, 0, 0, 0 );
   if ( (proc->state() != QProcess::NotRunning) ) { 
     proc->kill();
     removeTempFiles();
   }
   if ( jobStarted ) { 
     jobStarted=false;
     emit finished( "", false );
   }
}

bool compileJob::running() { 
  return jobStarted;
}

void compileJob::texJobFinished( int eCode, QProcess::ExitStatus eStat ) {
  pdfFName = texName2Pdf(tmpSRC.fileName());
  disconnect( proc, 0, 0, 0 );
  connect( proc, SIGNAL( finished(int,QProcess::ExitStatus) ), this, texJobFinished(int,QProcess::ExitStatus) ); 
  proc->start( gsPath, "-sDEVICE=bbox -dBATCH -dNOPAUSE -f "+pdfFName );
}

void compileJob::gsJobFinished( int eCode, QProcess::ExitStatus eStat ) {
  removeTempFiles();
  disconnect( proc, 0, 0, 0 );
  jobStarted=false;
  QRect bBox = parseGSOutput();
  emit finished( pdfFName, bBox, true );
}



  



  
  




renderItem::~renderItem() { 
  if ( pdf ) delete pdf;
  delete localLoop;
}

renderItem::renderItem( QString source, QString preamb ):
	src(source), pre(preamb), ready(false), bBox(0,0,0,0), job_id(-1), pdf(NULL),
	waiting_for_job(false)
{ 
  connect( &job, SIGNAL( ready(QString,bool) ), this, SLOT( pdfReady(QString,bool) ) );
  localLoop = new QEventLoop( this );
}

void renderItem::pdfReady( QString pdfFName, bool status ) { 
  if ( ! status ) {
    ready = false;
    qWarning() << "renderItem::pdfReady: Error compiling latex. Job failed.";
  } else { 
    if ( pdf ) delete pdf;
    pdf = Poppler::Document::load( pdfFName );
    pdf->setRenderHint( Poppler::Document::TextAntialiasing, true );
    pdf->setRenderHint( Poppler::Document::Antialiasing, true );
    ready = true;
  }
  if ( waiting_for_ready ) {
    waiting_for_job = false;
    localLoop->exit();
  }
  if ( status && job_id > -1 ) {
    emit ready( job_id );
    job_id = -1;
  }
}

void renderItem::wait_for_job() { 
  if ( waiting_for_job ) {
    qDebug() << "Warning, already waiting for job " << job_id;
  } else { 
    waiting_for_job = true;
    localLoop->exec();
  }
}

void renderItem::preRender( int jobID ) { 
  if ( job.running() ) { 
    if ( job_id == jobID ) return; // Probably already running, no need to run again.
    job.kill();
    job_id = jobID;
  }
  job.start( getLaTeX( src, pre ) );
}

 

void renderItem::updateItem( QString source, QString preabmule ) { 
  src = source;
  pre = preambule;
  if ( pdf ) delete pdf;
  pdf = NULL;
  bBox = QRectF(0,0,0,0);
  ready = false;
  if ( job.running() ) job.restart( getLaTeX( source, preabmule ) );
}

void renderItem::size() { 
  return bBox.width()*bBox.height();
}

QPixmap renderItem::render( qreal zoom, bool format_inline, QString latex, QString gs ) { 
  if ( job.running() ) wait_for_job();
  if ( ! ready ) { 
    job.start( getLaTeX( src, pre ) );
    wait_for_job();
  }
  if ( ready && pdf ) { 
    Poppler::Page *pg = pdf->page(0);
    QImage image = pg->renderToImage( 72*zoom, 72*zoom );
    delete pg;
    return QPixmap::fromImage( image );
  } else { 
    return QPixmap();
  }
}



#include "teXjob.moc"
