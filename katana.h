#include "Vector2f.h"
class katana{
    int itsRange;
    int itsDamage;
    float itsCooldown;
    bool itsReady;
public:
    int getDamage();
    int getRange();
    float getCooldown();
    bool isReady();
    void setDamage(int d);
    void setRange(int r);
    void setCooldown(float c);
    void launchAttack(Vector2f mouseWorldPos);
   };
