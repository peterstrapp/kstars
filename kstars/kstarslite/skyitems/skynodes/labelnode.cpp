/** *************************************************************************
                          pointsourcenode.cpp  -  K Desktop Planetarium
                             -------------------
    begin                : 16/06/2016
    copyright            : (C) 2016 by Artem Fedoskin
    email                : afedoskin3@gmail.com
 ***************************************************************************/
/** *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "skyobject.h"
#include "Options.h"

#include <QSGSimpleTextureNode>

#include "skymaplite.h"
#include "labelnode.h"

LabelNode::LabelNode(SkyObject * skyObject, LabelsItem::label_t type)
    :SkyNode(skyObject), m_name(skyObject->name()), m_textTexture(new QSGSimpleTextureNode),
      m_zoomFont(false), m_labelType(type), m_fontSize(0)
{
    switch(type) {
        case LabelsItem::label_t::PLANET_LABEL:
        case LabelsItem::label_t::SATURN_MOON_LABEL:
        case LabelsItem::label_t::JUPITER_MOON_LABEL:
        case LabelsItem::label_t::COMET_LABEL:
        case LabelsItem::label_t::RUDE_LABEL:
        case LabelsItem::label_t::ASTEROID_LABEL:
            m_zoomFont = true;
        break;
        default:
            break;
    }

    createTexture();
    m_opacity->appendChildNode(m_textTexture);

}

LabelNode::LabelNode(QString name, LabelsItem::label_t type)
    :m_name(name), m_textTexture(new QSGSimpleTextureNode), m_labelType(type), m_zoomFont(false)
{
    switch(type) {
        case LabelsItem::label_t::PLANET_LABEL:
        case LabelsItem::label_t::SATURN_MOON_LABEL:
        case LabelsItem::label_t::JUPITER_MOON_LABEL:
        case LabelsItem::label_t::COMET_LABEL:
        case LabelsItem::label_t::RUDE_LABEL:
        case LabelsItem::label_t::ASTEROID_LABEL:
            m_zoomFont = true;
        break;
        default:
            break;
    }

    createTexture();
    m_opacity->appendChildNode(m_textTexture);
}

void LabelNode::createTexture() {
    QColor color;

    switch(m_labelType) {
        case LabelsItem::label_t::SATURN_MOON_LABEL:
        case LabelsItem::label_t::JUPITER_MOON_LABEL:
            SkyLabeler::Instance()->shrinkFont(2);
            break;
        default:
            break;
    }

    switch(m_labelType) {
        case LabelsItem::label_t::PLANET_LABEL:
        case LabelsItem::label_t::SATURN_MOON_LABEL:
        case LabelsItem::label_t::JUPITER_MOON_LABEL:
        case LabelsItem::label_t::COMET_LABEL:
        case LabelsItem::label_t::RUDE_LABEL:
            color = KStarsData::Instance()->colorScheme()->colorNamed( "PNameColor" );
            break;
        case LabelsItem::label_t::ASTEROID_LABEL:
            color = QColor("gray");
            break;
        case LabelsItem::label_t::CONSTEL_NAME_LABEL:
            color = KStarsData::Instance()->colorScheme()->colorNamed( "CNameColor" );
            break;
        default:
            color = KStarsData::Instance()->colorScheme()->colorNamed( "UserLabelColor" );
    }

    QSGTexture *oldTexture = m_textTexture->texture();

    m_textTexture->setTexture(SkyMapLite::Instance()->textToTexture(m_name, color, m_zoomFont));
    if(m_zoomFont) m_fontSize = SkyLabeler::Instance()->drawFont().pointSize();

    switch(m_labelType) {
        case LabelsItem::label_t::SATURN_MOON_LABEL:
        case LabelsItem::label_t::JUPITER_MOON_LABEL:
            SkyLabeler::Instance()->resetFont();
            break;
        default:
            break;
    }

    if(oldTexture) delete oldTexture;

    m_textSize = m_textTexture->texture()->textureSize();
    QRectF oldRect = m_textTexture->rect();
    m_textTexture->setRect(QRect(oldRect.x(),oldRect.y(),m_textSize.width(),m_textSize.height()));
}

void LabelNode::changePos(QPointF pos) {
    QMatrix4x4 m (1,0,0,pos.x(),
                  0,1,0,pos.y(),
                  0,0,1,0,
                  0,0,0,1);
    //m.translate(-0.5*size.width(), -0.5*size.height());

    setMatrix(m);
    markDirty(QSGNode::DirtyMatrix);
}

void LabelNode::setLabelPos(QPointF pos) {
    show();
    //We need to subtract the height of texture from final y to follow the way QPainter draws the text
    if(m_skyObject) labelPos = QPointF(pos.x() + m_skyObject->labelOffset(), pos.y() + m_skyObject->labelOffset() - m_textSize.height());
    else labelPos = QPointF(pos.x()-(m_textSize.width()/2.0), pos.y());//QPointF(pos.x()-(m_textSize.width()/2.0), pos.y() + m_textSize.height());
}

void LabelNode::update() {
    if(m_zoomFont && m_fontSize != SkyLabeler::Instance()->skyFont().pointSize()) {
        createTexture();
    }
    changePos(labelPos);
}