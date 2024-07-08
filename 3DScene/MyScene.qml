import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Input 2.12

Entity {
id: root
Camera {
    id: camera
    projectionType: CameraLens.PerspectiveProjection
    fieldOfView: 45
    aspectRatio: 16/9
    nearPlane : 0.1
    farPlane : 1000.0
    position: Qt.vector3d(0.0, 0.0, 40.0)
    upVector: Qt.vector3d(0.0, 1.0, 0.0)
    viewCenter: Qt.vector3d(0.0, 0.0, 0.0)
}


components: [
    RenderSettings {

    },
    InputSettings {}
]



Transform {
    id: transform
    translation: Qt.vector3d(0, 0, 0)
}

Entity {
    components: [mesh, material, transform]
}
}
