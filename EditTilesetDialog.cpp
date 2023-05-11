#include "EditTilesetDialog.h"


namespace TilesEditor
{
	EditTilesetDialog::EditTilesetDialog(const QString& fileName, ResourceManager& resourceManager, QWidget* parent)
		: QDialog(parent), m_resourceManager(resourceManager)
	{
		ui.setupUi(this);
		m_selectedType = 0;
		connect(ui.graphicsView, &GraphicsView::renderView, this, &EditTilesetDialog::renderScene);
		connect(ui.graphicsView, &GraphicsView::mouseWheelEvent, this, &EditTilesetDialog::graphicsMouseWheel);
		connect(ui.graphicsView, &GraphicsView::mouseMove, this, &EditTilesetDialog::graphicsMouseMove);
		connect(ui.graphicsView, &GraphicsView::mousePress, this, &EditTilesetDialog::graphicsMouseMove);

		connect(ui.overlayButton, &QAbstractButton::clicked, ui.graphicsView, &GraphicsView::redraw);

		connect(ui.radioButtonType0, &QAbstractButton::clicked, this, &EditTilesetDialog::tileTypeClicked);
		connect(ui.radioButtonType1, &QAbstractButton::clicked, this, &EditTilesetDialog::tileTypeClicked);
		connect(ui.radioButtonType2, &QAbstractButton::clicked, this, &EditTilesetDialog::tileTypeClicked);
		connect(ui.radioButtonType3, &QAbstractButton::clicked, this, &EditTilesetDialog::tileTypeClicked);
		connect(ui.radioButtonType4, &QAbstractButton::clicked, this, &EditTilesetDialog::tileTypeClicked);
		connect(ui.radioButtonType5, &QAbstractButton::clicked, this, &EditTilesetDialog::tileTypeClicked);
		connect(ui.radioButtonType6, &QAbstractButton::clicked, this, &EditTilesetDialog::tileTypeClicked);
		connect(ui.radioButtonType7, &QAbstractButton::clicked, this, &EditTilesetDialog::tileTypeClicked);
		connect(ui.radioButtonType8, &QAbstractButton::clicked, this, &EditTilesetDialog::tileTypeClicked);
		connect(ui.radioButtonType9, &QAbstractButton::clicked, this, &EditTilesetDialog::tileTypeClicked);
		connect(ui.radioButtonType10, &QAbstractButton::clicked, this, &EditTilesetDialog::tileTypeClicked);
		connect(ui.radioButtonType11, &QAbstractButton::clicked, this, &EditTilesetDialog::tileTypeClicked);

		m_tileset.loadFromFile(fileName);

		m_tilesetImage = static_cast<Image*>(resourceManager.loadResource(m_tileset.getImageName(), ResourceType::RESOURCE_IMAGE));
			

		if (m_tilesetImage)
		{
			ui.graphicsView->setSceneRect(0, 0, m_tilesetImage->width(), m_tilesetImage->height());
		}

	}

	EditTilesetDialog::~EditTilesetDialog()
	{
		if (m_tilesetImage != nullptr)
			m_resourceManager.freeResource(m_tilesetImage);
	
	}

	void EditTilesetDialog::renderScene(QPainter* painter, const QRectF& rect)
	{
		static QColor tileColours[] = {
			QColorConstants::Svg::lime,
			QColorConstants::Svg::gray,
			QColorConstants::Yellow,
			QColorConstants::Blue,
			QColor(128, 64, 0),
			QColorConstants::Svg::teal,
			QColorConstants::Green,
			QColor(128, 128, 255),
			QColorConstants::Red,
			QColor(255, 128, 128),
			QColor(192, 255, 255),
			QColor(255, 128, 255)
		};

		static auto colourCount = sizeof(tileColours) / sizeof(QColor);

		if (m_tilesetImage)
		{
			painter->setOpacity(0.75);
			painter->drawPixmap(0, 0, m_tilesetImage->pixmap());
			painter->setOpacity(1.0);
			if (ui.overlayButton->isChecked())
			{
				for (auto top = 0; top < m_tileset.getVCount(); ++top)
				{
					for (auto left = 0; left < m_tileset.getHCount(); ++left)
					{
						auto tileType = m_tileset.getTileType(left, top);
					
						if (tileType >= 0 && tileType < colourCount) {
							auto colour = tileColours[tileType];
						
							painter->fillRect(left * 16, top * 16, 16, 16, QColor(colour.red(), colour.green(), colour.blue(), 100) );
						}

					}
				}
			}

			for (auto top = 0; top < m_tileset.getVCount(); ++top)
			{
				for (auto left = 0; left < m_tileset.getHCount(); ++left)
				{
					auto tileType = m_tileset.getTileType(left, top);

					if (tileType >= 0 && tileType < colourCount) {
						painter->drawText(left * 16, top * 16, 16, 16, Qt::AlignHCenter, QString::number(tileType));
					}

				}
			}
		}
	}

	void EditTilesetDialog::graphicsMouseWheel(QWheelEvent* event)
	{
		event->ignore();
	}

	void EditTilesetDialog::graphicsMouseMove(QMouseEvent* event)
	{
		if (event->buttons().testFlag(Qt::MouseButton::LeftButton))
		{
			auto pos = ui.graphicsView->mapToScene(event->pos());

			auto tileXPos = int(pos.x() / 16.0);
			auto tileYPos = int(pos.y() / 16.0);

			m_tileset.setTileType(tileXPos, tileYPos, m_selectedType);

			ui.graphicsView->redraw();
		}
	}

	void EditTilesetDialog::tileTypeClicked(bool checked)
	{
		if (checked)
		{
			auto widget = static_cast<QWidget*>(this->sender());

			m_selectedType = widget->toolTip().toInt();
			
		}
	}

};