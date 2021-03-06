#ifndef WOVERVIEWHSV_H
#define WOVERVIEWHSV_H

#include "widget/woverview.h"

class WOverviewHSV : public WOverview {
  public:
    WOverviewHSV(
            const char* group,
            PlayerManager* pPlayerManager,
            UserSettingsPointer pConfig,
            QWidget* parent = nullptr);

  private:
    bool drawNextPixmapPart() override;
};

#endif // WOVERVIEWHSV_H
