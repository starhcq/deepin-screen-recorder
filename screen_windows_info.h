#include <QObject>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>

class ScreenWindowsInfo : public QObject 
{
    Q_OBJECT
    
  public:
    xcb_window_t rootWindow;
    
    ScreenWindowsInfo(QObject *parent = 0);
    xcb_atom_t getAtom(QString name);
    xcb_get_property_reply_t* getProperty(xcb_window_t window, QString propertyName, xcb_atom_t type);
    QString getAtomName(xcb_atom_t atom);
    QStringList getWindowTypes(xcb_window_t window);
    QString getWindowName(xcb_window_t window);
    int getCurrentWorkspace(xcb_window_t window);
    int getWindowWorkspace(xcb_window_t window);
    QList<xcb_window_t> getWindows();
    xcb_get_geometry_reply_t* getWindowGeometry(xcb_window_t window);
    
  protected:
    xcb_connection_t* conn;
    xcb_screen_t* screen;
};
    
