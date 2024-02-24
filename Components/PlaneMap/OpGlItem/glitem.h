
#ifndef GLITEM_H
#define GLITEM_H

#include <QSGGeometryNode>
#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QImage>
#include <QOpenGLFunctions>
#include <QObject>

class GlItem: public QQuickItem, protected QOpenGLFunctions
{
 Q_OBJECT
  public:
    GlItem(QQuickItem *parent = nullptr);
    
  protected:
    virtual QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData) override;
    
  private:
    QImage current_image;
    
    QSGGeometryNode* m_borderActiveNode;
};

#endif // GLITEM_H
