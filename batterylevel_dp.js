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

        await port.open({ baudRate: 115200}); // เปิดพอร์ต
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
let batteryLevels = {}; // ใช้เก็บค่าของแต่ละแบตเตอรี่

async function readSerialData() {
    const decoder = new TextDecoderStream();
    port.readable.pipeTo(decoder.writable);
    reader = decoder.readable.getReader();

    try {
        while (keepReading) {
            const { value, done } = await reader.read();
            if (done) break;
            
            let cleanValue = value.trim();
            // console.log("📡 ข้อมูลที่ได้รับ: " + cleanValue);
            // console.log("🔋 แบตเตอรี่ A:", batteryLevels["A"]);
            // console.log("🔋 แบตเตอรี่ B:", batteryLevels["B"]);
            // console.log("🔋 แบตเตอรี่ C:", batteryLevels["C"]);
            // console.log("🔋 แบตเตอรี่ D:", batteryLevels["D"]);
            // console.log("🔋 แบตเตอรี่ E:", batteryLevels["E"]);

            // ถ้าไม่มีการรับข้อมูล ของ A, B, C, D, E

            if (cleanValue.startsWith("A")) {
                let number = cleanValue.substring(1); // ลบ "A-" ออก
                batteryLevels["A"] = Number(number);
                document.getElementById("battery-level-A").textContent = number;
                if (Number(number) <= 20) {
                    //console.log("⚠️ แบตเตอรี่ A ต่ำกว่า 20%!");
                    document.getElementById("battery-level-group-boxA").style.color = "red";
                    //document.getElementById("battery-level-group-boxA").style.color = "white";
                   // document.getElementById("mac-address-A").style.color = "white";
                    
                }else if (Number(number) >= 20 && Number(number) < 50) {
                    //console.log("⚠️ แบตเตอรี่ A ต่ํากว่า 50%!");
                    document.getElementById("battery-level-group-boxA").style.color  = "yellow";
                    //document.getElementById("battery-level-group-boxA").style.color = "white";
                }else if (Number(number) >= 50) {
                    //console.log("⚠️ แบตเตอรี่ A ต่ํากว่า 100%!");
                    document.getElementById("battery-level-group-boxA").style.color ="green";
                    //document.getElementById("battery-level-group-boxA").style.color = "white";
                }else{
                    //document.getElementById("battery-level-group-boxA").style.color ="white";
                    document.getElementById("battery-level-group-boxA").style.color = "black";
                    //document.getElementById("mac-address-A").style.color = "black";
                }
            }if (cleanValue.startsWith("B")) {
                let number = cleanValue.substring(1);
                batteryLevels["B"] = Number(number);
                document.getElementById("battery-level-B").textContent = number;
                if (Number(number) <= 20) {
                    //console.log("⚠️ แบตเตอรี่ B ต่ำกว่า 20%!");
                    document.getElementById("battery-level-group-boxB").style.color ="red";
                    //document.getElementById("battery-level-group-boxA").style.background = "white";
                   // document.getElementById("mac-address-A").style.color = "white";
                    
                }else if (Number(number) >= 20 && Number(number) < 50) {
                    //console.log("⚠️ แบตเตอรี่ B ต่ํากว่า 50%!");
                    document.getElementById("battery-level-group-boxB").style.color ="yellow";
                    //document.getElementById("battery-level-group-boxA").style.color = "white";
                }else if (Number(number) >= 50) {                    console.log("⚠️ แบตเตอรี่ B ต่ํากว่า 100%!");
                    document.getElementById("battery-level-group-boxB").style.color ="green";
                    //document.getElementById("battery-level-group-boxA").style.color = "white";
                }else{
                    //document.getElementById("battery-level-group-boxB").style.background ="white";
                    document.getElementById("battery-level-group-boxB").style.color = "black";
                    //document.getElementById("mac-address-A").style.color = "black";
                }
            }if (cleanValue.startsWith("C")) {
                let number = cleanValue.substring(1);
                batteryLevels["C"] = Number(number);
                document.getElementById("battery-level-C").textContent = number;
                if (Number(number) <= 20) {
                    //console.log("⚠️ แบตเตอรี่ C ต่ำกว่า 20%!");
                    document.getElementById("battery-level-group-boxC").style.color ="red";
                    //document.getElementById("battery-level-group-boxA").style.color = "white";
                   // document.getElementById("mac-address-A").style.color = "white";
                    
                }else if (Number(number) >= 20 && Number(number) < 50) {
                    //console.log("⚠️ แบตเตอรี่ C ต่ํากว่า 50%!");
                    document.getElementById("battery-level-group-boxC").style.color ="yellow";
                    //document.getElementById("battery-level-group-boxA").style.color = "white";
                }else if (Number(number) >= 50) {
                    //console.log("⚠️ แบตเตอรี่ C ต่ํากว่า 100%!");
                    document.getElementById("battery-level-group-boxC").style.color ="green";
                    //document.getElementById("battery-level-group-boxA").style.color = "white";
                }else{
                   //document.getElementById("battery-level-group-boxC").style.background ="white";
                    document.getElementById("battery-level-group-boxC").style.color = "black";
                    //document.getElementById("mac-address-A").style.color = "black";
                }
            }if (cleanValue.startsWith("D")) {
                let number = cleanValue.substring(1);
                batteryLevels["D"] = Number(number);
                document.getElementById("battery-level-D").textContent = number;
                if (Number(number) <= 20) {
                    //console.log("⚠️ แบตเตอรี่ D ต่ำกว่า 20%!");
                    document.getElementById("battery-level-group-boxD").style.color ="red";
                    //document.getElementById("battery-level-group-boxA").style.color = "white";
                   // document.getElementById("mac-address-A").style.color = "white";
                    
                }else if (Number(number) >= 20 && Number(number) < 50) {
                    //console.log("⚠️ แบตเตอรี่ D ต่ํากว่า 50%!");
                    document.getElementById("battery-level-group-boxD").style.color ="yellow";
                    //document.getElementById("battery-level-group-boxA").style.color = "white";
                }else if (Number(number) >= 50) {
                    //console.log("⚠️ แบตเตอรี่ D ต่ํากว่า 100%!");
                    document.getElementById("battery-level-group-boxD").style.color ="green";
                    //document.getElementById("battery-level-group-boxA").style.color = "white";
                }else{
                    //document.getElementById("battery-level-group-boxD").style.background ="white";
                    document.getElementById("battery-level-group-boxD").style.color = "black";
                    //document.getElementById("mac-address-A").style.color = "black";
                }
            }if (cleanValue.startsWith("E")) {
                let number = cleanValue.substring(1);
                batteryLevels["E"] = Number(number);
                document.getElementById("battery-level-E").textContent = number;
                if (Number(number) <= 20) {
                    //console.log("⚠️ แบตเตอรี่ E ต่ำกว่า 20%!");
                    document.getElementById("battery-level-group-boxE").style.color ="red";
                    //document.getElementById("battery-level-group-boxA").style.color = "white";
                   // document.getElementById("mac-address-A").style.color = "white";
                    
                }else if (Number(number) >= 20 && Number(number) < 50) {
                    //console.log("⚠️ แบตเตอรี่ E ต่ํากว่า 50%!");
                    document.getElementById("battery-level-group-boxE").style.color ="yellow";
                    //document.getElementById("battery-level-group-boxA").style.color = "white";
                }else if (Number(number) >= 50) {
                    //console.log("⚠️ แบตเตอรี่ E ต่ํากว่า 100%!");
                    document.getElementById("battery-level-group-boxE").style.color ="green";
                    //document.getElementById("battery-level-group-boxA").style.color = "white";
                }else{
                    //document.getElementById("battery-level-group-boxE").style.background ="white";
                    document.getElementById("battery-level-group-boxE").style.color = "black";
                    //document.getElementById("mac-address-A").style.color = "black";
                }
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
