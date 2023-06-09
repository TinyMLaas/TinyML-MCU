from flask import Flask, abort, request, jsonify
from relay.components.install import install_inference
from relay.components.device import get_device_port, get_connected_devices
from relay.observing import read_prediction_from_port


app = Flask(__name__)
port = 5000


@app.route('/install/', methods=['POST'])
def install():
    if request.method == 'POST':
        if "device" not in request.get_json():
            return "No device in request", 400
        res = request.get_json()
        device = res["device"]
        model = res["model"]

        if not install_inference(device, model):
            return f"Device with installer \"{device['installer']}\" is not supported"

        return 'Success', 201
    else:
        abort(400)


@app.route('/prediction/', methods=['GET'])
def get_prediction():
    device = request.get_json()["device"]["serial"]
    if not device:
        return "No device selected in request", 400
    device_port = get_device_port(device)

    pred = read_prediction_from_port(device_port)

    if not pred:
        return "Failed to read prediction from device", 404

    return pred  # jsonify(pred)


@app.route('/devices/', methods=['GET'])
def get_devices():
    devices = get_connected_devices()

    response = {"devices": devices}

    return jsonify(response), 200


@app.route('/health/', methods=['GEt'])
def health():
    return 'Online', 200


if __name__ == "__main__":
    app.run()
