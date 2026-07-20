#include "hxc/program.h"

_Static_assert(offsetof(struct hxc_caxecraft_domain_BlockCoord, hxc_x) == 0, "closed record hxc_caxecraft_domain_BlockCoord first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_BlockCoord) >= _Alignof(int32_t), "closed record hxc_caxecraft_domain_BlockCoord alignment admits field 0");

_Static_assert(offsetof(struct hxc_caxecraft_domain_BlockCoord, hxc_y) >= offsetof(struct hxc_caxecraft_domain_BlockCoord, hxc_x) + sizeof(int32_t), "closed record hxc_caxecraft_domain_BlockCoord field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_BlockCoord) >= _Alignof(int32_t), "closed record hxc_caxecraft_domain_BlockCoord alignment admits field 1");

_Static_assert(offsetof(struct hxc_caxecraft_domain_BlockCoord, hxc_z) >= offsetof(struct hxc_caxecraft_domain_BlockCoord, hxc_y) + sizeof(int32_t), "closed record hxc_caxecraft_domain_BlockCoord field 2 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_BlockCoord) >= _Alignof(int32_t), "closed record hxc_caxecraft_domain_BlockCoord alignment admits field 2");

_Static_assert(sizeof(struct hxc_caxecraft_domain_BlockCoord) >= offsetof(struct hxc_caxecraft_domain_BlockCoord, hxc_z) + sizeof(int32_t), "closed record hxc_caxecraft_domain_BlockCoord size contains its final field");

_Static_assert(offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_cellX) == 0, "closed record hxc_caxecraft_domain_RaycastHit first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_RaycastHit) >= _Alignof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit alignment admits field 0");

_Static_assert(offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_cellY) >= offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_cellX) + sizeof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_RaycastHit) >= _Alignof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit alignment admits field 1");

_Static_assert(offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_cellZ) >= offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_cellY) + sizeof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit field 2 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_RaycastHit) >= _Alignof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit alignment admits field 2");

_Static_assert(offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_distance) >= offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_cellZ) + sizeof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit field 3 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_RaycastHit) >= _Alignof(double), "closed record hxc_caxecraft_domain_RaycastHit alignment admits field 3");

_Static_assert(offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_hit) >= offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_distance) + sizeof(double), "closed record hxc_caxecraft_domain_RaycastHit field 4 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_RaycastHit) >= _Alignof(bool), "closed record hxc_caxecraft_domain_RaycastHit alignment admits field 4");

_Static_assert(offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_normalX) >= offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_hit) + sizeof(bool), "closed record hxc_caxecraft_domain_RaycastHit field 5 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_RaycastHit) >= _Alignof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit alignment admits field 5");

_Static_assert(offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_normalY) >= offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_normalX) + sizeof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit field 6 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_RaycastHit) >= _Alignof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit alignment admits field 6");

_Static_assert(offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_normalZ) >= offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_normalY) + sizeof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit field 7 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_RaycastHit) >= _Alignof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit alignment admits field 7");

_Static_assert(offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_previousX) >= offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_normalZ) + sizeof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit field 8 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_RaycastHit) >= _Alignof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit alignment admits field 8");

_Static_assert(offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_previousY) >= offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_previousX) + sizeof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit field 9 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_RaycastHit) >= _Alignof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit alignment admits field 9");

_Static_assert(offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_previousZ) >= offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_previousY) + sizeof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit field 10 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_RaycastHit) >= _Alignof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit alignment admits field 10");

_Static_assert(offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_visited) >= offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_previousZ) + sizeof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit field 11 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_RaycastHit) >= _Alignof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit alignment admits field 11");

_Static_assert(sizeof(struct hxc_caxecraft_domain_RaycastHit) >= offsetof(struct hxc_caxecraft_domain_RaycastHit, hxc_visited) + sizeof(int32_t), "closed record hxc_caxecraft_domain_RaycastHit size contains its final field");

_Static_assert(offsetof(struct hxc_caxecraft_domain_StepInput, hxc_jump) == 0, "closed record hxc_caxecraft_domain_StepInput first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_StepInput) >= _Alignof(bool), "closed record hxc_caxecraft_domain_StepInput alignment admits field 0");

_Static_assert(offsetof(struct hxc_caxecraft_domain_StepInput, hxc_moveX) >= offsetof(struct hxc_caxecraft_domain_StepInput, hxc_jump) + sizeof(bool), "closed record hxc_caxecraft_domain_StepInput field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_StepInput) >= _Alignof(double), "closed record hxc_caxecraft_domain_StepInput alignment admits field 1");

_Static_assert(offsetof(struct hxc_caxecraft_domain_StepInput, hxc_moveZ) >= offsetof(struct hxc_caxecraft_domain_StepInput, hxc_moveX) + sizeof(double), "closed record hxc_caxecraft_domain_StepInput field 2 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_StepInput) >= _Alignof(double), "closed record hxc_caxecraft_domain_StepInput alignment admits field 2");

_Static_assert(sizeof(struct hxc_caxecraft_domain_StepInput) >= offsetof(struct hxc_caxecraft_domain_StepInput, hxc_moveZ) + sizeof(double), "closed record hxc_caxecraft_domain_StepInput size contains its final field");

_Static_assert(offsetof(struct hxc_caxecraft_domain_AxisMove, hxc_blocked) == 0, "closed record hxc_caxecraft_domain_AxisMove first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_AxisMove) >= _Alignof(bool), "closed record hxc_caxecraft_domain_AxisMove alignment admits field 0");

_Static_assert(offsetof(struct hxc_caxecraft_domain_AxisMove, hxc_x) >= offsetof(struct hxc_caxecraft_domain_AxisMove, hxc_blocked) + sizeof(bool), "closed record hxc_caxecraft_domain_AxisMove field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_AxisMove) >= _Alignof(double), "closed record hxc_caxecraft_domain_AxisMove alignment admits field 1");

_Static_assert(offsetof(struct hxc_caxecraft_domain_AxisMove, hxc_y) >= offsetof(struct hxc_caxecraft_domain_AxisMove, hxc_x) + sizeof(double), "closed record hxc_caxecraft_domain_AxisMove field 2 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_AxisMove) >= _Alignof(double), "closed record hxc_caxecraft_domain_AxisMove alignment admits field 2");

_Static_assert(offsetof(struct hxc_caxecraft_domain_AxisMove, hxc_z) >= offsetof(struct hxc_caxecraft_domain_AxisMove, hxc_y) + sizeof(double), "closed record hxc_caxecraft_domain_AxisMove field 3 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_AxisMove) >= _Alignof(double), "closed record hxc_caxecraft_domain_AxisMove alignment admits field 3");

_Static_assert(sizeof(struct hxc_caxecraft_domain_AxisMove) >= offsetof(struct hxc_caxecraft_domain_AxisMove, hxc_z) + sizeof(double), "closed record hxc_caxecraft_domain_AxisMove size contains its final field");

_Static_assert(offsetof(struct hxc_caxecraft_domain_PlayerState, hxc_grounded) == 0, "closed record hxc_caxecraft_domain_PlayerState first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_PlayerState) >= _Alignof(bool), "closed record hxc_caxecraft_domain_PlayerState alignment admits field 0");

_Static_assert(offsetof(struct hxc_caxecraft_domain_PlayerState, hxc_velocityX) >= offsetof(struct hxc_caxecraft_domain_PlayerState, hxc_grounded) + sizeof(bool), "closed record hxc_caxecraft_domain_PlayerState field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_PlayerState) >= _Alignof(double), "closed record hxc_caxecraft_domain_PlayerState alignment admits field 1");

_Static_assert(offsetof(struct hxc_caxecraft_domain_PlayerState, hxc_velocityY) >= offsetof(struct hxc_caxecraft_domain_PlayerState, hxc_velocityX) + sizeof(double), "closed record hxc_caxecraft_domain_PlayerState field 2 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_PlayerState) >= _Alignof(double), "closed record hxc_caxecraft_domain_PlayerState alignment admits field 2");

_Static_assert(offsetof(struct hxc_caxecraft_domain_PlayerState, hxc_velocityZ) >= offsetof(struct hxc_caxecraft_domain_PlayerState, hxc_velocityY) + sizeof(double), "closed record hxc_caxecraft_domain_PlayerState field 3 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_PlayerState) >= _Alignof(double), "closed record hxc_caxecraft_domain_PlayerState alignment admits field 3");

_Static_assert(offsetof(struct hxc_caxecraft_domain_PlayerState, hxc_x) >= offsetof(struct hxc_caxecraft_domain_PlayerState, hxc_velocityZ) + sizeof(double), "closed record hxc_caxecraft_domain_PlayerState field 4 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_PlayerState) >= _Alignof(double), "closed record hxc_caxecraft_domain_PlayerState alignment admits field 4");

_Static_assert(offsetof(struct hxc_caxecraft_domain_PlayerState, hxc_y) >= offsetof(struct hxc_caxecraft_domain_PlayerState, hxc_x) + sizeof(double), "closed record hxc_caxecraft_domain_PlayerState field 5 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_PlayerState) >= _Alignof(double), "closed record hxc_caxecraft_domain_PlayerState alignment admits field 5");

_Static_assert(offsetof(struct hxc_caxecraft_domain_PlayerState, hxc_z) >= offsetof(struct hxc_caxecraft_domain_PlayerState, hxc_y) + sizeof(double), "closed record hxc_caxecraft_domain_PlayerState field 6 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_caxecraft_domain_PlayerState) >= _Alignof(double), "closed record hxc_caxecraft_domain_PlayerState alignment admits field 6");

_Static_assert(sizeof(struct hxc_caxecraft_domain_PlayerState) >= offsetof(struct hxc_caxecraft_domain_PlayerState, hxc_z) + sizeof(double), "closed record hxc_caxecraft_domain_PlayerState size contains its final field");

_Static_assert(hxc_caxecraft_domain_BlockKind_Air == 0, "enum hxc_caxecraft_domain_BlockKind case Air retains its Haxe discriminant");

_Static_assert(hxc_caxecraft_domain_BlockKind_Grass == 1, "enum hxc_caxecraft_domain_BlockKind case Grass retains its Haxe discriminant");

_Static_assert(hxc_caxecraft_domain_BlockKind_Dirt == 2, "enum hxc_caxecraft_domain_BlockKind case Dirt retains its Haxe discriminant");

_Static_assert(hxc_caxecraft_domain_BlockKind_Stone == 3, "enum hxc_caxecraft_domain_BlockKind case Stone retains its Haxe discriminant");

_Static_assert(hxc_caxecraft_domain_BlockKind_Bedrock == 4, "enum hxc_caxecraft_domain_BlockKind case Bedrock retains its Haxe discriminant");
