const formDecode = document.getElementById("form-decode");
const formEncode = document.getElementById("form-encode");

const baseUrl = "http://127.0.0.1:5000/"
formDecode.addEventListener("submit", async function(event) {
  event.preventDefault();
  let stringToDecode = formDecode.elements[0].value;
  if (!stringToDecode) {
    alert("Provide a valid hexadecimal");
    return;
  }
  if (stringToDecode.length != 10) {
    alert("Invalid hexadecimal. Provide an 8 digit hexadecimal starting with 0x that corresponds to a MIPS instruction. Ex: 0x08040000");
    return;
  }
  if (!stringToDecode.startsWith("0x")) {
    alert("Ensure your hexadecimal starts with 0x");
    return;
  }

  const formData = new URLSearchParams();
  formData.append("hexadecimal", stringToDecode);
  let url = baseUrl + "decode";
  fetch(url, {
    method: "POST",
    headers: {
      'Content-type': 'application/x-www-form-urlencoded',
    },
    mode: "cors",
    body: formData.toString()
  }).then(response => response.text())
    .then(data => {
      const result = document.getElementById("decode-result");
      result.value = data;
      console.log(result);
    })
    .catch(err => console.error(err))
});

formEncode.addEventListener("submit", async function(event) {
  event.preventDefault();
  let stringToEncode = formEncode.elements[0].value;

  const formData = new URLSearchParams();
  formData.append("instruction", stringToEncode);
  let url = baseUrl + "encode";
  fetch(url, {
    method: "POST",
    headers: {
      'Content-type': 'application/x-www-form-urlencoded',
    },
    mode: "cors",
    body: formData.toString()
  }).then(response => response.text())
    .then(data => {
      const res = document.getElementById("encode-result");
      res.value = data;
      console.log(res);
    })
    .catch(err => console.error(err))
});


