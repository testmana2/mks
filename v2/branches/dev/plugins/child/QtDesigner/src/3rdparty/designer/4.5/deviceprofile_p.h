/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of Qt Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef DEVICEPROFILE_H
#define DEVICEPROFILE_H

#include "shared_global_p.h"

#include <QtCore/QString>
#include <QtCore/QSharedDataPointer>

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class QWidget;
class QStyle;

namespace qdesigner_internal {

class DeviceProfileData;

/* DeviceProfile for embedded design. They influence
 * default properties (for example, fonts), dpi and
 * style of the form. This class represents a device
 * profile. */

class QDESIGNER_SHARED_EXPORT DeviceProfile {
public:
    DeviceProfile();

    DeviceProfile(const DeviceProfile&);
    DeviceProfile& operator=(const DeviceProfile&);
    ~DeviceProfile();

    void clear();

    // Device name
    QString name() const;
    void setName(const QString &);

    // System settings active
    bool isEmpty() const;

    // Default font family of the embedded system
    QString fontFamily() const;
    void setFontFamily(const QString &);

    // Default font size of the embedded system
    int fontPointSize() const;
    void setFontPointSize(int p);

    // Display resolution of the embedded system
    int dpiX() const;
    void setDpiX(int d);
    int dpiY() const;
    void setDpiY(int d);

    // Style
    QString style() const;
    void setStyle(const QString &);

    // Initialize from desktop system
    void fromSystem();

    static void systemResolution(int *dpiX, int *dpiY);
    static void widgetResolution(const QWidget *w, int *dpiX, int *dpiY);

    bool equals(const DeviceProfile& rhs) const;

    // Apply to form/preview (using font inheritance)
    enum ApplyMode {
        /* Pre-Apply to parent widget of form being edited: Apply font
         * and make use of property inheritance to be able to modify the
         * font property freely. */
        ApplyFormParent,
        /* Post-Apply to preview widget: Change only inherited font
         * sub properties. */
        ApplyPreview
    };
    void apply(const QDesignerFormEditorInterface *core, QWidget *widget, ApplyMode am) const;

    static void applyDPI(int dpiX, int dpiY, QWidget *widget);

    QString toString() const;

    QString toXml() const;
    bool fromXml(const QString &xml, QString *errorMessage);

private:
    QSharedDataPointer<DeviceProfileData> m_d;
};

inline bool operator==(const DeviceProfile &s1, const DeviceProfile &s2)
    { return s1.equals(s2); }
inline bool operator!=(const DeviceProfile &s1, const DeviceProfile &s2)
    { return !s1.equals(s2); }

}


QT_END_NAMESPACE

#endif // DEVICEPROFILE_H
