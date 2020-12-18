/*
 * Copyright (c) 2020 <copyright holder> <email>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <QtWidgets/QWidget>

#include "obsremotesettings.hpp"
#include "obsremoteabout.hpp"
#include "ui_obsremotesettings.h"
#include <qglobal.h>

namespace OBSRemote::Frontend {
OBSRemoteSettings::OBSRemoteSettings(QWidget* parent) : QWidget(parent, Qt::Dialog), m_ui(new Ui::OBSRemoteSettings), config(new Config) {
	m_ui->setupUi(this);
	
	connect(m_ui->OkBtn, &QPushButton::clicked, this,  &OBSRemoteSettings::FormAccepted);
	connect(m_ui->CancelBtn, &QPushButton::clicked, this,  &OBSRemoteSettings::FormCanceled);
	connect(m_ui->AboutBtn, &QPushButton::clicked, this,  &OBSRemoteSettings::OpenAbout);
}

void OBSRemoteSettings::OpenAbout() {
	OBSRemoteAbout* about_ = new OBSRemoteAbout(QWidget::find( this->effectiveWinId()));
	about_->show();
}

void OBSRemoteSettings::FormAccepted() {
	
}

void OBSRemoteSettings::FormCanceled() {
	close();
}

void OBSRemoteSettings::showEvent([[maybe_unused]] QShowEvent* event) {
	m_ui->ServerEnableChBox->setChecked(true);
}

void OBSRemoteSettings::ToggleShowHide() {
	if (!isVisible()) {
		setVisible(true);
	} else {
		setVisible(false);
	}
}
}
