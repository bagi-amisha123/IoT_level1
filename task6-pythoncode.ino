import requests
import matplotlib.pyplot as plt
import datetime

# ThingSpeak read API information
THINGSPEAK_CHANNEL_ID = "your_channel_id"
THINGSPEAK_READ_API_KEY = "your_read_api_key"
THINGSPEAK_URL = f"https://api.thingspeak.com/channels/{THINGSPEAK_CHANNEL_ID}/fields/1.json"

# Get data from ThingSpeak
def fetch_data():
    url = f"{THINGSPEAK_URL}?api_key={THINGSPEAK_READ_API_KEY}&results=100"
    response = requests.get(url).json()
    temperatures = [float(entry['field1']) for entry in response['feeds'] if entry['field1']]
    timestamps = [datetime.datetime.strptime(entry['created_at'], "%Y-%m-%dT%H:%M:%SZ") for entry in response['feeds']]
    return timestamps, temperatures

# Plot temperature data
timestamps, temperatures = fetch_data()
plt.plot(timestamps, temperatures, marker='o')
plt.title("Temperature vs Time")
plt.xlabel("Time")
plt.ylabel("Temperature (°C)")
plt.xticks(rotation=45)
plt.tight_layout()
plt.show()