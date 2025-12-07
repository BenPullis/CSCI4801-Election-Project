const express = require('express');
const path = require('path');
const multer = require('multer');
const app = express();
const port = 8081;

const storage = multer.diskStorage({
    destination: (req, file, cb) => {
        cb(null, path.join(__dirname, 'uploads'));
    },
    filename: (req, file, cb) => {
        cb(null, Date.now() + '-' + file.originalname);
    }
});

const upload = multer({ storage: storage });

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});

app.post('/upload', upload.array('myfiles'), (req, res) => {
    if (!req.files || req.files.length === 0) {
        return res.status(400).send('No file uploaded.');
    }

    res.send(`
        <html>
            <body>
                <p>Upload successful! Please close this tab and return to your command-line!</p>
            </body>
        </html>
    `);
});

app.use(express.static(__dirname));

app.listen(port, () => { });
