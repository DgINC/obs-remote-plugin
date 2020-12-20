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
#include <QtNetwork/QNetworkInterface>

#include "obsremotesettings.hpp"
#include "obsremoteabout.hpp"
#include "ui_obsremotesettings.h"

namespace OBSRemote::Frontend {
OBSRemoteSettings::OBSRemoteSettings(QWidget* parent) : QWidget(parent, Qt::Dialog), m_ui(new Ui::OBSRemoteSettings), config(new Config) {
	m_ui->setupUi(this);
	
	connect(m_ui->OkBtn, &QPushButton::clicked, this,  &OBSRemoteSettings::FormAccepted);
	connect(m_ui->CancelBtn, &QPushButton::clicked, this,  &OBSRemoteSettings::FormCanceled);
	connect(m_ui->AboutBtn, &QPushButton::clicked, this,  &OBSRemoteSettings::OpenAbout);
	
	foreach (const QNetworkInterface &netInterface, QNetworkInterface::allInterfaces()) {
		QNetworkInterface::InterfaceFlags flags = netInterface.flags();
		if( (bool)(flags & QNetworkInterface::IsRunning) && !(bool)(flags & QNetworkInterface::IsLoopBack) && (bool)(flags & QNetworkInterface::CanMulticast)){
			foreach (const QNetworkAddressEntry &address, netInterface.addressEntries()) {
				if(address.ip().protocol() == QAbstractSocket::IPv4Protocol)
					eths << address.ip().toString();
				}
		}
	}
	m_ui->InterfacesComBox->addItems(eths);
}

OBSRemoteSettings::~OBSRemoteSettings() {

}

void OBSRemoteSettings::OpenAbout() {
	OBSRemoteAbout* about_ = new OBSRemoteAbout(QWidget::find( this->effectiveWinId()));
	about_->show();
}

void OBSRemoteSettings::FormAccepted() {
	config->server_enable_ = m_ui->ServerEnableChBox->isChecked();
	config->dns_sd_enable_ = m_ui->DNSDEnableChBox->isChecked();
	config->port_ = m_ui->ServerPortBox->value();
	config->Save();
	close();
}

void OBSRemoteSettings::FormCanceled() {
	close();
}

void OBSRemoteSettings::showEvent([[maybe_unused]] QShowEvent* event) {
	m_ui->ServerEnableChBox->setChecked(config->server_enable_);
	m_ui->DNSDEnableChBox->setChecked(config->dns_sd_enable_);
	m_ui->ServerPortBox->setValue(config->port_);
	
}

void OBSRemoteSettings::ToggleShowHide() {
	setVisible(!isVisible());
}
}
