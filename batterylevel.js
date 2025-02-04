let port;
let reader;
let keepReading = false;

const selectButton = document.getElementById("select-port");
const disconnectButton = document.getElementById("disconnect-port");

// 🟢 1️⃣ เลือกพอร์ต
selectButton.addEventListener("click", async () => {
    try {
        if (!port) {
            port = await navigator.serial.requestPort(); // เปิด UI ให้เลือกพอร์ต
        }

        if (port.readable) {
            alert("⚠️ พอร์ตถูกเปิดอยู่แล้ว!");
            return;
        }

        await port.open({ baudRate: 9600 }); // เปิดพอร์ต
        keepReading = true;
        readSerialData(); // อ่านข้อมูล

        console.log("✅ เชื่อมต่อพอร์ตสำเร็จ");
        disconnectButton.disabled = false; // เปิดปุ่มตัดการเชื่อมต่อ
    } catch (error) {
        console.error("❌ เลือกพอร์ตไม่สำเร็จ", error);
        alert("❌ เกิดข้อผิดพลาด: " + error.message);
    }
});

// 🟡 2️⃣ อ่านค่าจากพอร์ตและแสดงผลใน alert
async function readSerialData() {
    const decoder = new TextDecoderStream();
    port.readable.pipeTo(decoder.writable);
    reader = decoder.readable.getReader();

    try {
        while (keepReading) {
            const { value, done } = await reader.read();
            if (done) break;

            // แสดงข้อมูลที่ได้รับใน Alert Popup
            console.log("📡 ข้อมูลที่ได้รับ: " + value);

            // ตรวจจับค่าที่ขึ้นต้นด้วย "A"
            if (value.startsWith("A")) {
                // นำค่าที่ได้รับไปแสดงใน element ที่มี id="battery-level-A"
                document.getElementById("battery-level-A").textContent = value.substring(1); // ลบ "A" ออก
            }else if (value.startsWith("B")) {
                // นำค่าที่ได้รับไปแสดงใน element ที่มี id="battery-level-B"
                document.getElementById("battery-level-B").textContent = value.substring(1); // ลบ "B" ออก
            }else if (value.startsWith("C")) {
                // นำค่าที่ได้รับไปแสดงใน element ที่มี id="battery-level-C"
                document.getElementById("battery-level-C").textContent = value.substring(1); // ลบ "C" ออก
            }else if (value.startsWith("D")) {
                // นำค่าที่ได้รับไปแสดงใน element ที่มี id="battery-level-D"
                document.getElementById("battery-level-D").textContent = value.substring(1); // ลบ "D" ออก
            }else if (value.startsWith("E")) {
                // นำค่าที่ได้รับไปแสดงใน element ที่มี id="battery-level-E"
                document.getElementById("battery-level-E").textContent = value.substring(1); // ลบ "E" ออก
            }
            
        }
    } catch (error) {
        console.error("❌ เกิดข้อผิดพลาดในการอ่าน:", error);
        alert("❌ อ่านค่าล้มเหลว: " + error.message);
    }
}

// 🔴 3️⃣ ตัดการเชื่อมต่อพอร์ต
disconnectButton.addEventListener("click", async () => {
    try {
        keepReading = false;
        if (reader) await reader.cancel();
        if (port) await port.close();
        
        console.log("🔴 ตัดการเชื่อมต่อพอร์ตแล้ว");
        alert("🔴 ตัดการเชื่อมต่อพอร์ตแล้ว");

        disconnectButton.disabled = true; // ปิดปุ่มตัดการเชื่อมต่อ
    } catch (error) {
        console.error("❌ ตัดการเชื่อมต่อล้มเหลว", error);
        alert("❌ ตัดการเชื่อมต่อล้มเหลว: " + error.message);
    }
});

// 🟠 4️⃣ ปิดพอร์ตเมื่อออกจากหน้าเว็บ
window.addEventListener("beforeunload", async () => {
    keepReading = false;
    if (reader) await reader.cancel();
    if (port) await port.close();
});
