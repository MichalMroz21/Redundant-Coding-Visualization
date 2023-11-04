    import QtQuick
import QtQuick.Window
import QtQuick.Controls

Window {
    id: window
    width: 1280
    height: 720
    visible: true
    title: qsTr("Redundant Coding")

    Grid {
        id: windowGrid
        x: 0
        y: 0
        width: 1280
        height: 720

        Grid {
            id: menuGrid
            width: windowGrid.width*0.3
            height: windowGrid.height
            verticalItemAlignment: Grid.AlignVCenter
            horizontalItemAlignment: Grid.AlignHCenter
            spacing: 20
            leftPadding: 50
            topPadding: 300
            rows: 5
            columns: 1

            Button {
                id: confirmButton
                text: qsTr("Zatwierdź")
            }

            Text {
                id: selectedAlgorithmText
                width: 300
                height: 22
                text: "Wybrany algorytm:"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }


            Text {
                id: algorithmText
                width: 300
                text: qsTr("Hamming")
                font.pixelSize: 20
                horizontalAlignment: Text.AlignHCenter
                topPadding: 0
            }

            Slider {
                id: slider
                snapMode: RangeSlider.SnapOnRelease
                stepSize: 1
                to: 2
                value: 0
            }




            Button {
                id: exitButton
                text: qsTr("Wyjście")
            }




        }

        Grid {
            id: descriptionGrid
            width: windowGrid.width * 0.7
            height: windowGrid.height
            rows: 2
            columns: 1

            Grid {
                id: imageGrid
                width: descriptionGrid.width
                height: descriptionGrid.height * 0.6

                Image {
                    id: image
                    width: imageGrid.width
                    height: imageGrid.height
                    source: "../assets/test.png"
                    fillMode: Image.PreserveAspectFit
                }
            }

            Grid {
                id: textGrid
                width: descriptionGrid.width
                height: descriptionGrid.height * 0.4
                columns: 1
                rows: 5

                Text {
                    id: descriptionText
                    width: textGrid.width
                    height: textGrid.height
                    text: qsTr("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.")
                    font.pixelSize: 24
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                    wrapMode: Text.WordWrap
                }
            }
        }
    }
}
