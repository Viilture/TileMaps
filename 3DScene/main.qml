
import QtQuick         2.12
import Qt3D.Core       2.12
import Qt3D.Render     2.12
import Qt3D.Input      2.12
import Qt3D.Extras     2.12
import QtQuick.Scene3D 2.12


Item {
    Scene3D {
        id: scene3d
        anchors.fill: parent
        //anchors.margins: 10
        focus: true
        aspects: ["input", "logic"]
        cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

        Entity {
            id: root
          //  objectName: "root"

            components: [
                RenderSettings {

                    activeFrameGraph: ForwardRenderer {
                        camera: mainCamera
                        clearColor: "grey"
                    }
                },
                // Event Source will be set by the Qt3DQuickWindow
                InputSettings { }
            ]

            BasicCamera {
                id: mainCamera
                position: Qt.vector3d( -5.17253, 2.95727, 6.65948 )
                viewCenter: Qt.vector3d( 0,0,0)
            }



            FirstPersonCameraController { camera: mainCamera }

            Lights { }


            Entity {
                id: floor2

                PropertyAnimation {
                    id: animation
                    target: floorT
                    property: "rotationY"
                    from: 0
                    to: 360
                    duration: 10000
                }


                components: [
                    Transform {id: floorT
                        //                           // translation: Qt.vector3d(0, 0, -10) // Перемещаем сферу на расстояние 10 единиц по z-оси
                        //
                        scale: 0.1
                    },
                    Mesh {
                        source: "qrc:/3DScene/drone_costum.obj"
                    },
                    TexturedMetalRoughMaterial {

                        baseColor:  TextureLoader {
                            id: textureLoader
                            source: "qrc:/3DScene/image.png"
                            format: Texture.SRGB8_Alpha8
                            generateMipMaps: true
                        }
                    }
                ]
            }

            Entity {
                id: floor22

                PropertyAnimation {
                    id: animation2
                    target: floorT2
                    property: "rotationY"
                    from: 0
                    to: 360
                    duration: 10000
                }
                PropertyAnimation {id: animation3; target: floorT2; property: "translation.x";from: 4;to: 3; duration: 5000;}
                PropertyAnimation {id: animation4; target: floorT2; property: "translation.z";from: 4;to: 2; duration: 5000;}
                components: [
                    Transform {id: floorT2
                        //                           // translation: Qt.vector3d(0, 0, -10) // Перемещаем сферу на расстояние 10 единиц по z-оси

                        scale: 0.1
                    },
                    Mesh
                    {
                        source: "qrc:/3DScene/drone_costum.obj"
                    },
                    TexturedMetalRoughMaterial {

                        baseColor:  TextureLoader {
                            id: textureLoader2
                            source: "qrc:/3DScene/image.png"
                            format: Texture.SRGB8_Alpha8
                            generateMipMaps: true
                        }
                    }
                ]
            }

            //                Entity {
            //                    components: [
            //                        Transform {
            //                           // translation: Qt.vector3d(0, 0, -10) // Перемещаем сферу на расстояние 10 единиц по z-оси
            //                        },
            //                        SphereMesh {
            //                            radius: 2 // Задаем радиус сферы
            //                            rings: 50  // Количество частей по широте
            //                            slices: 50 // Количество частей по долготе
            //                        },
            //                        TexturedMetalRoughMaterial { // Материал для отображения сферы

            //                            baseColor: Qt.rgba(1, 0, 0, 0.01)
            //                        }
            //                    ]
            //                }
            Entity {
                id: floor

//                Timer {
//                    interval: 3000; running: true; repeat: true
//                    onTriggered: {

//                        //textureLoader.source = "file:///E:/MYFILE/Program/WORK/MapLib/build-Barier2ARM-Desktop_Qt_5_12_12_MinGW_32_bit-Release/image2.jpg?" + Math.random()
//                        console.log("Hello from QML!")
//                        animation.start();
//                        animation3.start();
//                        animation4.start();
//                    }
//                }

                components: [
                    PlaneMesh {
                        width: 19
                        height: 10
                        mirrored: true
                        PlaneGeometry {
                            //facetCount: 500 // Большее кол-во граней - более детализированная поверхность
                            width: 19
                            height: 10

                        }
                    },
                    TexturedMetalRoughMaterial {
                        id: material
                        baseColor:  TextureLoader {
                            id: textureLoader4
                            source: "qrc:/3DScene/image.png"
                            format: Texture.SRGB8_Alpha8
                            generateMipMaps: true
                        }
                    }
                ]
            }
        }
    }
}


