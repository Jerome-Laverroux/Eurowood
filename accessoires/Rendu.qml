import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Input 2.12
import Qt3D.Extras 2.12

import QtQuick 2.12 as QQ2

import "3D"




Entity {
    property var barre:{"largeur":100,"hauteur":200,"longueur":5000}
    property int  marge: 5
    id: sceneRoot

    Camera {
        id: camera
        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 45
        nearPlane : 0.1
        farPlane : 1000.0
        position: Qt.vector3d( 0.0,-( (barre.longueur/200)+marge), 0.0 )
        upVector: Qt.vector3d( 0.0, 0.0, 1.0 )
        viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
    }

    FirstPersonCameraController { camera: camera }

    components: [
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                camera: camera
                clearColor: Qt.rgba(0.8,0.8,0.8,0.8)
            }
        },
        InputSettings { }
    ]

    PhongMaterial {
        id: material
    }

    PhongMaterial{
        id:poutre
        diffuse: "red"
        ambient: "red"
    }

    Appuis{}

    CuboidMesh{
        id:cube
        xExtent: barre.largeur
        yExtent: barre.hauteur
        zExtent: barre.longueur


    }
    Transform{
        id:cubetransform
        rotationY: 90
        rotationZ: 90
        scale3D: Qt.vector3d(0.01,0.01,0.01)

    }

    Entity{
        id:cubeentity
        components: [cube,poutre,cubetransform]
    }



    TorusMesh {
        id: torusMesh
        radius: 5
        minorRadius: 1
        rings: 100
        slices: 20
    }

    Transform {
        id: torusTransform
        scale3D: Qt.vector3d(1.5, 1, 0.5)
        rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 45)
    }

    Entity {
        id: torusEntity
        components: [ torusMesh, material, torusTransform ]
    }

    SphereMesh {
        id: sphereMesh
        radius: 3
    }

    Transform {
        id: sphereTransform
        property real userAngle: 0.0
        matrix: {
            var m = Qt.matrix4x4();
            m.rotate(userAngle, Qt.vector3d(0, 1, 0))
            m.translate(Qt.vector3d(20, 0, 0));
            return m;
        }
    }

    QQ2.NumberAnimation {
        target: sphereTransform
        property: "userAngle"
        duration: 10000
        from: 0
        to: 360

        loops: QQ2.Animation.Infinite
        running: true
    }

    Entity {
        id: sphereEntity
        components: [ sphereMesh, material, sphereTransform ]
    }
}
