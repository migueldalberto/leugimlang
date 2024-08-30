const body = document.querySelector("body")
const log_div = document.createElement("div")
body.appendChild(log_div)

function print (str) {
	const p = document.createElement("p")
	p.textContent = str
	log_div.appendChild(p)
	log_div.setAttribute("class", "log")

	if (log_div.children.length > 64) {
		log_div.removeChild(log_div.firstChild)
	}
}
