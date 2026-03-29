import { app, Menu, dialog, ipcMain } from 'electron'

let debugEnabled = false

function buildMenu (win) {
  const isMac = process.platform === 'darwin'

  const template = [
    // App menu (macOS only)
    ...(isMac
      ? [{
          label: app.name,
          submenu: [
            { role: 'about' },
            { type: 'separator' },
            { role: 'hide' },
            { role: 'hideOthers' },
            { role: 'unhide' },
            { type: 'separator' },
            { role: 'quit' }
          ]
        }]
      : []),

    // Edit
    {
      label: 'Edit',
      submenu: [
        { role: 'undo' },
        { role: 'redo' },
        { type: 'separator' },
        { role: 'cut' },
        { role: 'copy' },
        { role: 'paste' },
        { role: 'selectAll' }
      ]
    },

    // View
    {
      label: 'View',
      submenu: [
        { role: 'reload' },
        { role: 'forceReload' },
        { role: 'toggleDevTools' },
        { type: 'separator' },
        { role: 'resetZoom' },
        { role: 'zoomIn' },
        { role: 'zoomOut' },
        { type: 'separator' },
        { role: 'togglefullscreen' },
        { type: 'separator' },
        {
          label: 'Debug Mode',
          type: 'checkbox',
          checked: debugEnabled,
          click: (menuItem) => {
            debugEnabled = menuItem.checked
            if (win && !win.isDestroyed()) {
              win.webContents.send('set-debug', debugEnabled)
            }
          }
        }
      ]
    },

    // Window
    {
      label: 'Window',
      submenu: [
        { role: 'minimize' },
        { role: 'zoom' },
        ...(isMac
          ? [
              { type: 'separator' },
              { role: 'front' }
            ]
          : [
              { role: 'close' }
            ])
      ]
    },

    // Help
    {
      label: 'Help',
      submenu: [
        ...(!isMac
          ? [{
              label: 'About MIDIPedal Control',
              click: () => {
                dialog.showMessageBox(win, {
                  type: 'info',
                  title: 'About MIDIPedal Control',
                  message: 'MIDIPedal Control',
                  detail: `Version 0.0.1\n\nConfiguration tool for MIDIPedal interfaces.\n\nElectron ${process.versions.electron}\nNode ${process.versions.node}\nChrome ${process.versions.chrome}`,
                  buttons: ['OK']
                })
              }
            }]
          : [])
      ]
    }
  ]

  const menu = Menu.buildFromTemplate(template)
  Menu.setApplicationMenu(menu)
}

// Sync debug state from renderer on startup
ipcMain.on('debug-changed', (_event, value) => {
  debugEnabled = value
})

// Notify renderer to release serial port before quitting
let mainWindow = null

app.on('before-quit', () => {
  if (mainWindow && !mainWindow.isDestroyed()) {
    mainWindow.webContents.send('app-before-quit')
  }
})

// Quit when all windows are closed.
app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit()
})

// Load here all startup windows
const winHandler = require('./mainWindow').default

winHandler.onCreated((browserWindow) => {
  mainWindow = browserWindow
  buildMenu(browserWindow)
})
