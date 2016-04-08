#include "OMPVarInfo.h"

OMPVarInfo::OMPVarInfo(clang::ValueDecl *setDecl, enum CAPTURE_TYPE setType,
        bool setIsGlobal) {
    decl = setDecl;
    type = setType;
    isGlobal = setIsGlobal;
}

enum CAPTURE_TYPE OMPVarInfo::getType() {
    return type;
}

clang::ValueDecl *OMPVarInfo::getDecl() {
    return decl;
}

bool OMPVarInfo::checkIsGlobal() {
    return isGlobal;
}

bool OMPVarInfo::isPassedByReference() {
    return !isGlobal && type == CAPTURE_TYPE::SHARED;
}
