
#include "glitem.h"


#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QSGVertexColorMaterial>
#include <QSGSimpleTextureNode>
#include <QImageReader>

GlItem::GlItem(QQuickItem *parent) :  QQuickItem(parent)
{
    QImageReader reader("C:/Projects/QT_Test/QtTest1/Components/PlaneMap/loadedCrew.jpg");
    this->current_image = reader.read();
    
    setFlag( QQuickItem::ItemHasContents, true);
    
}

/*
QSGNode* GlItem::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{    
    QSGGeometryNode *node; 
    if (!oldNode) {
        node = new QSGGeometryNode(); 
        
               // Создаём объект для геометрии
        QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(),3);
        geometry->setDrawingMode(QSGGeometry::DrawTriangleFan);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
        node->setFlag(QSGNode::OwnsOpaqueMaterial);
        
               // Задаём текстуру и материал
         
        auto texture = window()->createTextureFromImage(current_image);
        auto material = new QSGOpaqueTextureMaterial;
        material->setTexture(texture);
        material->setFiltering(QSGTexture::Linear);
        material->setMipmapFiltering(QSGTexture::Linear);
        node->setMaterial(material);
        
        node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    } else {
        node = static_cast<QSGSimpleTextureNode *>(oldNode);
//        node = oldNode;
        node->markDirty(QSGNode::DirtyGeometry);
    }
    
    
    
    return node;
}
*/


QSGNode* GlItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data)
{
    qDebug() << "=============================================================";    
    QSGSimpleTextureNode *node = static_cast<QSGSimpleTextureNode *>(oldNode);
    if(node == nullptr) {
        //        QSGGeometry* borderNonActiveGeometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 360);
        node = new QSGSimpleTextureNode();
        node->setRect(boundingRect());
    }

    QSGTexture *texture = window()->createTextureFromImage(current_image);
    node->setTexture(texture);
    
   
//    node->setOwnsTexture(true);
    QRectF bounding_rect = boundingRect();
    qDebug() << "w:" << bounding_rect.width() << "h:" << bounding_rect.height();    
    
    node->setRect(boundingRect());
    node->markDirty(QSGNode::DirtyForceUpdate);
    node->setTexture(texture);
    
    return node;
}

/*
QSGNode *GlItem::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    QSGGeometryNode *node = 0;
    QSGGeometry *geometry = 0;
    
    if (!oldNode) {
        node = new QSGGeometryNode;
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_ColoredPoint2D(), data.size());
        geometry->setDrawingMode(GL_POINTS);
//            setDrawingModeelsize(GL_POINTS);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
        
        QSGVertexColorMaterial *material = new QSGVertexColorMaterial();
        //material->setColor(QColor(255, 0, 0));
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
        
    } else {
        node = static_cast<QSGGeometryNode *>(oldNode);
        geometry = node->geometry();
        geometry->allocate(222);
    }
    QSGGeometry::ColoredPoint2D *vertices = geometry->vertexDataAsColoredPoint2D();
    for (int i = 0; i < data.size(); i++) {
        vertices[id].set(x, y,red, green, blue, 255);
    }
}
}
node->markDirty(QSGNode::DirtyGeometry);
return node;
    
}*/


