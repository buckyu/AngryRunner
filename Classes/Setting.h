#define PTM_RATIO 32.0f

#define SCREEN_TO_WORLD(n) ((n) / PTM_RATIO)
#define WORLD_TO_SCREEN(n) ((n) * PTM_RATIO)
#define B2_ANGLE_TO_COCOS_ROTATION(n) (-1 * CC_RADIANS_TO_DEGREES(n))
#define COCOS_ROTATION_TO_B2_ANGLE(n) (CC_DEGREES_TO_RADIANS(-1 * n))

#define OBJ_TYPE_GROUND     011
#define OBJ_TYPE_PLAYER     001
#define OBJ_TYPE_BONUS      002
#define OBJ_TYPE_TRAP       003
#define PLAYER_MAX_SPEED    020
#define PLAYER_FORW_IMPLS   050
#define TRAP_NONE           999
#define TRAP_SAW            100
#define TRAP_TNT            101
#define TRAP_MAGNET         102
#define TRAP_SWORD          103
#define TRAP_GOD            104
#define TRAP_BUBLE          105
#define TRAP_MANTRAP        106

#define POWER_SPEED         201
#define POWER_ROCKET        202
#define POWER_SHIELD        203

#define OWNER_MY            300

#define PLAYER_SHIELD_TIME   5
