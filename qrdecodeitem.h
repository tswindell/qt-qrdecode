/*
    Copyright (C) 2011-2016 Imogen Software Carsten Valdemar Munk

    Contact: Tom Swindell <t.swindell@rubyx.co.uk>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef QRDECODEITEM_H
#define QRDECODEITEM_H

#include <QQuickItem>

class QVideoFrame;

class QRDecodeItem : public QQuickItem
{
    Q_OBJECT

    Q_ENUMS(SymbolType)

    Q_PROPERTY(QObject *source READ source WRITE setSource NOTIFY sourceChanged)

public:
    typedef enum {
        EAN8        =      8,  /**< EAN-8 */
        UPCE        =      9,  /**< UPC-E */
        ISBN10      =     10,  /**< ISBN-10 (from EAN-13). @since 0.4 */
        UPCA        =     12,  /**< UPC-A */
        EAN13       =     13,  /**< EAN-13 */
        ISBN13      =     14,  /**< ISBN-13 (from EAN-13). @since 0.4 */
        I25         =     25,  /**< Interleaved 2 of 5. @since 0.4 */
        CODE39      =     39,  /**< Code 39. @since 0.4 */
        PDF417      =     57,  /**< PDF417. @since 0.6 */
        QRCODE      =     64,  /**< QR Code. @since 0.10 */
        CODE128     =    128,  /**< Code 128 */
    } SymbolType;

    explicit QRDecodeItem(QQuickItem *parent = 0);
            ~QRDecodeItem();

    QObject* source() const;
    Q_INVOKABLE void setSource(QObject *source);

Q_SIGNALS:
    void decoded(SymbolType type, const QString &data);

    void error(const QString &error);
    void sourceChanged();

public Q_SLOTS:
    void start();
    void stop();

protected Q_SLOTS:
    void onVideoFrame(const QVideoFrame &frame);

protected:
    void timerEvent(QTimerEvent *event);

private:
    class QRDecodeItemPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QRDecodeItem)
};

#endif // QRDECODEITEM_H
