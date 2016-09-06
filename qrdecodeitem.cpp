#include "qrdecodeitem.h"

#include <QCamera>
#include <QVideoProbe>

#include <QDebug>

#include "zbar.h"

class QRDecodeItemPrivate : public zbar::Image::Handler
{
    QRDecodeItem *q_ptr;
    Q_DECLARE_PUBLIC(QRDecodeItem)

public:
    QRDecodeItemPrivate(QRDecodeItem *q)
        : q_ptr(q), intervalId(-1), probe(NULL), source(NULL), buffer(NULL), scanner(NULL)
    { /* ... */ }

    int intervalId;

    QVideoProbe *probe;

    QObject *source;

    zbar::Image *buffer;
    zbar::ImageScanner *scanner;

    //FIXME: This needs to run in its' own thread.
    void image_callback(zbar::Image &image)
    {
        Q_Q(QRDecodeItem);
        for(zbar::Image::SymbolIterator iter = image.symbol_begin(); iter != image.symbol_end(); ++iter)
        {
            if(iter->get_count()) continue;

            QRDecodeItem::SymbolType type = static_cast<QRDecodeItem::SymbolType>(iter->get_type());
            QString data = QString::fromStdString(iter->get_data());

            emit q->decoded(type, data);
        }
    }
};

QRDecodeItem::QRDecodeItem(QQuickItem *parent)
    : QQuickItem(parent), d_ptr(new QRDecodeItemPrivate(this))
{
    Q_D(QRDecodeItem);

    d->probe = new QVideoProbe(this);

    d->scanner = new zbar::ImageScanner;
    d->scanner->set_handler(*d_ptr);
}

QRDecodeItem::~QRDecodeItem()
{
    Q_D(QRDecodeItem);

    if(d->scanner != NULL) delete d->scanner;

    delete d_ptr;
}

QObject* QRDecodeItem::source() const
{
    Q_D(const QRDecodeItem);
    return d->source;
}

void QRDecodeItem::setSource(QObject *source)
{
    Q_D(QRDecodeItem);
    if(d->source != source)
    {
        QCamera *camera = qvariant_cast<QCamera*>(source->property("mediaObject"));
        if(camera == NULL) {
            qDebug() << "Passed value is not a valid QCamera instance!";
            emit error("Source is not a valid QCamera instance!");
            return;
        }

        if(!d->probe->setSource(camera))
        {
            qDebug() << "Failed to assign media player interface!";
            emit error("Failed to assign media player interface to QVideoProbe.");
            return;
        }

        d->source = source;
        emit sourceChanged();
    }
}

void QRDecodeItem::start()
{
    Q_D(QRDecodeItem);

    QObject::connect(d->probe, SIGNAL(videoFrameProbed(QVideoFrame)), this, SLOT(onVideoFrame(QVideoFrame)));

    if(d->intervalId != -1) return;
    d->intervalId = this->startTimer(1000);
}

void QRDecodeItem::stop()
{
    Q_D(QRDecodeItem);

    QObject::disconnect(d->probe, SIGNAL(videoFrameProbed(QVideoFrame)), this, SLOT(onVideoFrame(QVideoFrame)));

    if(d->intervalId != -1)
    {
        this->killTimer(d->intervalId);
        d->intervalId = -1;
    }

    if(d->buffer != NULL)
    {
        delete d->buffer;
        d->buffer = NULL;
    }
}

void QRDecodeItem::onVideoFrame(const QVideoFrame &pFrame)
{
    Q_D(QRDecodeItem);
    QVideoFrame frame = pFrame;

    if(frame.map(QAbstractVideoBuffer::ReadOnly))
    {
        //TODO: Add more video format support.
        if(frame.pixelFormat() == QVideoFrame::Format_NV21)
        {
            // If buffer is still waiting to be processed, then ignore this frame.
            if(d->buffer != NULL) return;

            d->buffer = new zbar::Image(frame.width(), frame.height(), "NV21",
                                        frame.bits(), frame.mappedBytes());
        } else {
            qDebug() << "Unknown video frame format: " << frame.pixelFormat();
            emit error(QString("Unknown video frame format: %1").arg(frame.pixelFormat()));
        }
    } else {
        qDebug() << "Failed to map video frame buffer.";
        emit error(QString("Failed to map video frame buffer."));
    }
}

//FIXME: Instead of running in a timer, use a thread instead.
void QRDecodeItem::timerEvent(QTimerEvent *event)
{
    Q_D(QRDecodeItem);
    if(d->intervalId != event->timerId()) return;
    if(d->buffer == NULL) return;

    zbar::Image tmp = d->buffer->convert(*(long*)"Y800");
    d->scanner->scan(tmp);

    delete d->buffer;
    d->buffer = NULL;
}
