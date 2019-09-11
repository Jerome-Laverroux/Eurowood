import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Input 2.12
import Qt3D.Extras 2.12

import QtQuick 2.12 as QQ2


Entity{

    PhongMaterial{
        id:app
        diffuse: "black"
        ambient: "black"
    }


    Transform{
        id:appuistrans
        rotationX: 90
        translation: Qt.vector3d(0,0,-5)
    }
    ConeMesh{
        id:cone
        bottomRadius: 2.5
        length: 2.5
    }

    id:cubeentity
    components: [cone,app,appuistrans]



}


