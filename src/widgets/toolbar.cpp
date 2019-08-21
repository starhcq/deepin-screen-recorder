/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Maintainer: Peng Hui<penghui@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "toolbar.h"
#include "../utils/baseutils.h"
#include "../utils/saveutils.h"

#include <QPainter>
#include <QDebug>
#include <QApplication>
#include <QCursor>
#include <QGraphicsDropShadowEffect>
#include <dgraphicsgloweffect.h>
#include <QTimer>
#include <QSettings>

DWIDGET_USE_NAMESPACE

namespace {
    const int TOOLBAR_HEIGHT = 47;
    const int TOOLBAR_WIDTH = 452;

    const QSize TOOLBAR_WIDGET_SIZE = QSize(450, 45);
    const int BUTTON_SPACING = 3;
    const int BTN_RADIUS = 3;
}

ToolBarWidget::ToolBarWidget(QWidget *parent)
    :DBlurEffectWidget(parent),
      m_expanded(false)
{
    setBlurRectXRadius(10);
    setBlurRectYRadius(10);
    setRadius(30);
    setMode(DBlurEffectWidget::GaussianBlur);
    setBlurEnabled(true);
    setBlendMode(DBlurEffectWidget::InWindowBlend);
    setMaskColor(QColor(255, 255, 255, 140));
    //设置透明效果
//    setMaskAlpha(0);
//    setMaskColor(DBlurEffectWidget::LightColor);
    setFixedSize(TOOLBAR_WIDGET_SIZE);

    qDebug() << "~~~~~~" << this->size();
    m_hSeparatorLine = new QLabel(this);
    m_hSeparatorLine->setObjectName("HorSeparatorLine");
    m_hSeparatorLine->setFixedHeight(1);

//    m_majToolbar = new MajToolBar(this);
//    m_subToolbar = new SubToolBar(this);

    m_mainTool = new MainToolWidget(this);
    m_subTool = new SubToolWidget(this);

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setMargin(0);
    hLayout->setSpacing(2);
    hLayout->addWidget(m_mainTool, 0, Qt::AlignLeft);
    hLayout->addWidget(m_subTool, 1, Qt::AlignLeft);
//    vLayout->addWidget(m_hSeparatorLine, 0, Qt::AlignVCenter);
//    vLayout->addWidget(m_subToolbar, 0, Qt::AlignVCenter);
    setLayout(hLayout);

//    m_hSeparatorLine->hide();
//    m_subToolbar->hide();
//    m_subTool->hide();

    connect(m_mainTool, &MainToolWidget::buttonChecked, this, &ToolBarWidget::setExpand);
    connect(m_subTool, &SubToolWidget::keyBoardButtonClicked, this, &ToolBarWidget::keyBoardCheckedSlot);
    connect(m_subTool, SIGNAL(microphoneActionChecked(bool)), this, SIGNAL(microphoneActionCheckedSignal(bool)));
    connect(m_subTool, SIGNAL(systemAudioActionChecked(bool)), this, SIGNAL(systemAudioActionCheckedSignal(bool)));
//    connect(m_majToolbar, &MajToolBar::saveImage, this, &ToolBarWidget::saveImage);
//    connect(m_subToolbar, &SubToolBar::saveAction, this, &ToolBarWidget::saveImage);

//    connect(m_subToolbar, &SubToolBar::currentColorChanged, this, &ToolBarWidget::colorChanged);
//    connect(m_subToolbar, &SubToolBar::currentColorChanged,  m_majToolbar, &MajToolBar::mainColorChanged);

//    connect(m_subToolbar, &SubToolBar::showSaveTip, m_majToolbar, &MajToolBar::showSaveTooltip);
//    connect(m_subToolbar, &SubToolBar::hideSaveTip, m_majToolbar, &MajToolBar::hideSaveTooltip);

//    connect(this, &ToolBarWidget::shapePressed, m_majToolbar, &MajToolBar::shapePressed);
//    connect(this, &ToolBarWidget::saveBtnPressed, m_subToolbar, &SubToolBar::saveBtnPressed);
//    connect(m_majToolbar, &MajToolBar::saveSpecificedPath, this, &ToolBarWidget::saveSpecifiedPath);
//    connect(m_majToolbar, &MajToolBar::closed, this, &ToolBarWidget::closed);
}

void ToolBarWidget::paintEvent(QPaintEvent *e) {
    DBlurEffectWidget::paintEvent(e);

    QPainter painter(this);
    painter.setPen(QColor(255, 255, 255, 76.5));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawLine(QPointF(BTN_RADIUS, 0), QPointF(this->width() - 1, 0));
}

void ToolBarWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    QSettings settings(this);
    settings.beginGroup("common");
    bool expand = settings.value("expand_savelist", false).toBool();
    settings.endGroup();

    if (expand)
        QTimer::singleShot(0, this, [=] { setExpand(expand, "saveList"); });

    DBlurEffectWidget::showEvent(event);
}

bool ToolBarWidget::isButtonChecked() {
    return m_expanded;
}

void ToolBarWidget::specifiedSavePath() {
    m_majToolbar->specificedSavePath();
}

void ToolBarWidget::keyBoardCheckedSlot(bool checked)
{
    emit keyBoardCheckedSignal(checked);
}

void ToolBarWidget::setExpand(bool expand, QString shapeType) {
//    m_subToolbar->switchContent(shapeType);
    m_subTool->switchContent(shapeType);
//    emit expandChanged(expand, shapeType);

//    if (expand) {
//        m_expanded = true;
//        setFixedSize(TOOLBAR_WIDGET_SIZE.width(),
//                                 TOOLBAR_WIDGET_SIZE.height()*2+1);
//        m_hSeparatorLine->show();
//        m_subToolbar->show();
//    }
    emit changeFunctionSignal(shapeType);
    update();
}

ToolBarWidget::~ToolBarWidget() {}


ToolBar::ToolBar(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(TOOLBAR_WIDTH, TOOLBAR_HEIGHT);
    m_toolbarWidget = new ToolBarWidget(this);
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(1, 1, 1, 1);
    vLayout->addStretch();
    vLayout->addWidget(m_toolbarWidget);
    vLayout->addStretch();
    setLayout(vLayout);

//    QPushButton *button = new QPushButton("test", this);

//    QTimer::singleShot(3000, [button] {
//        qDebug() << button->palette() << button->palette().dark().color().name(QColor::HexArgb);
//        QPalette pa = button->palette();
//        pa.setColor(QPalette::ButtonText, Qt::red);
//        pa.setColor(QPalette::Light, Qt::blue);
//        pa.setColor(QPalette::Dark, pa.dark().color());
//        button->setPalette(pa);
//        qDebug() << button->palette() << button->palette().dark().color().name(QColor::HexArgb);
//        qDebug() << button->topLevelWidget();
//        qDebug() << button->topLevelWidget()->palette().dark().color().name(QColor::HexArgb);
//    });
//    button->topLevelWidget()->installEventFilter(this);

//    qDebug() << button->topLevelWidget()->palette().dark().color().name(QColor::HexArgb);

//    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
//    shadowEffect->setXOffset(0);
//    shadowEffect->setYOffset(6);
//    shadowEffect->setColor(QColor(0, 0, 0, 26));
//    shadowEffect->setBlurRadius(20);
//    setGraphicsEffect(shadowEffect);

    connect(m_toolbarWidget, &ToolBarWidget::expandChanged, this, &ToolBar::setExpand);
    connect(m_toolbarWidget, &ToolBarWidget::saveImage, this, &ToolBar::requestSaveScreenshot);
    connect(m_toolbarWidget, &ToolBarWidget::colorChanged, this, &ToolBar::updateColor);
    connect(this, &ToolBar::shapePressed, m_toolbarWidget, &ToolBarWidget::shapePressed);
    connect(this, &ToolBar::saveBtnPressed, m_toolbarWidget, &ToolBarWidget::saveBtnPressed);
    connect(m_toolbarWidget, &ToolBarWidget::saveSpecifiedPath, this, &ToolBar::saveSpecifiedPath);
    connect(m_toolbarWidget, &ToolBarWidget::closed, this, &ToolBar::closed);

    connect(m_toolbarWidget, &ToolBarWidget::changeFunctionSignal, this, &ToolBar::currentFunctionMode);
    connect(m_toolbarWidget, &ToolBarWidget::keyBoardCheckedSignal, this, &ToolBar::keyBoardCheckedToMainSlot);
    connect(m_toolbarWidget, &ToolBarWidget::microphoneActionCheckedSignal, this, &ToolBar::microphoneActionCheckedToMainSlot);
    connect(m_toolbarWidget, &ToolBarWidget::systemAudioActionCheckedSignal, this, &ToolBar::systemAudioActionCheckedToMainSlot);
}

void ToolBar::setExpand(bool expand, QString shapeType) {
    emit buttonChecked(shapeType);
    if (expand) {
        m_expanded = true;
        setFixedSize(TOOLBAR_WIDTH,
                              TOOLBAR_WIDGET_SIZE.height()*2+3);
        emit heightChanged();
    }

    update();
}

void ToolBar::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    painter.setPen(QColor(0, 0, 0, 25));
    painter.setRenderHint(QPainter::Antialiasing);
    QRectF rect(0, 0, this->width() -1, this->height() - 1);
    painter.drawRoundedRect(rect.translated(0.5, 0.5), 3, 3, Qt::AbsoluteSize);

    QLabel::paintEvent(e);
}

void ToolBar::enterEvent(QEvent *e) {
    qApp->setOverrideCursor(Qt::ArrowCursor);
    QLabel::enterEvent(e);
}

bool ToolBar::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::ApplicationPaletteChange || event->type() == QEvent::PaletteChange) {
        qDebug() << obj;
        qDebug() << "--------------";
    }

    return QLabel::eventFilter(obj, event);
}

void ToolBar::showAt(QPoint pos) {
    if (!isVisible())
        this->show();

    move(pos.x(), pos.y());
}

void ToolBar::specificedSavePath()
{
    m_toolbarWidget->specifiedSavePath();
}

void ToolBar::currentFunctionMode(QString shapeType)
{
    emit currentFunctionToMain(shapeType);
}

void ToolBar::keyBoardCheckedToMainSlot(bool checked)
{
    emit keyBoardCheckedToMain(checked);
}

void ToolBar::microphoneActionCheckedToMainSlot(bool checked){
    emit microphoneActionCheckedToMain(checked);
}
void ToolBar::systemAudioActionCheckedToMainSlot(bool checked){
    emit systemAudioActionCheckedToMain(checked);
}
bool ToolBar::isButtonChecked() {
    return m_expanded;
}

ToolBar::~ToolBar()
{
}