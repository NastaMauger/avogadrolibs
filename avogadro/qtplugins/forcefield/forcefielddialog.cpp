/******************************************************************************
  This source file is part of the Avogadro project.
  This source code is released under the 3-Clause BSD License, (see "LICENSE").
******************************************************************************/

#include "forcefield.h"
#include "forcefielddialog.h"
#include "ui_forcefielddialog.h"
#include "parser_forcefield.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QDir>   
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QWidget>


#include <cmath> // for log10

namespace Avogadro {
namespace QtPlugins {

ForceFieldDialog::ForceFieldDialog(const QStringList& forceFields,
                                   QWidget* parent_)
  : QDialog(parent_), ui(new Ui::ForceFieldDialog)
{
  ui->setupUi(this);
  ui->forceField->addItems(forceFields);
  updateRecommendedForceField();

  connect(ui->useRecommended, SIGNAL(toggled(bool)),
          SLOT(useRecommendedForceFieldToggled(bool)));

  ui->browseFileButton->setVisible(Forcefield::polarizedForceField);

  connect(ui->browseFileButton, &QPushButton::clicked, this, &ForceFieldDialog::browseFile);

  qDebug() << polarizedForceField ;

  // Initialize pointers to the widgets
  labelParameterSet = ui->label_ParameterSet;
  labelParameterSetHint = ui->label_ParameterSetHint;

  // Set the initial visibility based on polarizedForceField
  labelParameterSet->setVisible(Forcefield::polarizedForceField);
  labelParameterSetHint->setVisible(Forcefield::polarizedForceField);

  QSettings settings;
  bool autoDetect =
    settings.value("openbabel/optimizeGeometry/autoDetect", true).toBool();
  ui->useRecommended->setChecked(autoDetect);
}

ForceFieldDialog::~ForceFieldDialog()
{
  delete ui;
}

QVariantMap ForceFieldDialog::prompt(QWidget* parent_,
                                     const QStringList& forceFields,
                                     const QVariantMap& startingOptions,
                                     const QString& recommendedForceField_)
{
  ForceFieldDialog dlg(forceFields, parent_);
  dlg.setOptions(startingOptions);
  dlg.setRecommendedForceField(recommendedForceField_);

  QVariantMap options;
  if (static_cast<DialogCode>(dlg.exec()) == Accepted)
    options = dlg.options();

  return options;
}

QVariantMap ForceFieldDialog::options() const
{
  QVariantMap opts;

  opts["forcefield"] = ui->forceField->currentText();
  opts["maxSteps"] = ui->stepLimit->value();
  opts["tolerance"] = pow(10, ui->energyConv->value());
  opts["gradientTolerance"] = pow(10, ui->gradConv->value());
  opts["autodetect"] = ui->useRecommended->isChecked();

  return opts;
}

void ForceFieldDialog::setOptions(const QVariantMap& opts)
{
  if (opts.contains("forcefield") && opts["forcefield"].canConvert<QString>())
    ui->forceField->setCurrentText(opts["forcefield"].toString());
  if (opts.contains("maxSteps") && opts["maxSteps"].canConvert<int>())
    ui->stepLimit->setValue(opts["maxSteps"].toInt());
  if (opts.contains("tolerance") && opts["tolerance"].canConvert<double>())
    ui->energyConv->setValue(log10(opts["tolerance"].toDouble()));
  if (opts.contains("gradientTolerance") &&
      opts["gradientTolerance"].canConvert<double>())
    ui->gradConv->setValue(log10(opts["gradientTolerance"].toDouble()));
  if (opts.contains("autodetect") && opts["autodetect"].canConvert<bool>())
    ui->useRecommended->setChecked(opts["autodetect"].toBool());
}

void ForceFieldDialog::setRecommendedForceField(const QString& rff)
{
  if (rff == m_recommendedForceField)
    return;

  if (ui->forceField->findText(rff) == -1)
    return;

  m_recommendedForceField = rff;
  updateRecommendedForceField();
}

void ForceFieldDialog::useRecommendedForceFieldToggled(bool state)
{
  if (!m_recommendedForceField.isEmpty()) {
    if (state) {
      int index = ui->forceField->findText(m_recommendedForceField);
      if (index >= 0) {
        ui->forceField->setCurrentIndex(index);
      }
    }
  }
  ui->forceField->setEnabled(!state);

  QSettings().setValue("forcefield/autoDetect", state);
}

void ForceFieldDialog::updateRecommendedForceField()
{
  if (m_recommendedForceField.isEmpty()) {
    ui->useRecommended->hide();
    ui->forceField->setEnabled(true);
  } else {
    ui->useRecommended->setText(
      tr("Autodetect (%1)").arg(m_recommendedForceField));
    // Force the combo box to update if needed:
    useRecommendedForceFieldToggled(ui->useRecommended->isChecked());
    ui->useRecommended->show();
  }
}


void ForceFieldDialog::browseFile() {
    qDebug() << "polarizedForceField Value: " << Forcefield::polarizedForceField;
    QString filePath;
    QString fileContent = ParserForceField::loadAndParseFile(filePath);

    if (!fileContent.isEmpty()) {
        QString fileName = QFileInfo(filePath).fileName();
        ui->browseFileButton->setText(tr("%1").arg(fileName));
    }
}

} // namespace QtPlugins
} // namespace Avogadro
