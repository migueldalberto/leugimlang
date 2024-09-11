const log_div = document.createElement("div")
document.body.appendChild(log_div)

function escrever (str) {
	const p = document.createElement("p")
	p.textContent = str
	log_div.appendChild(p)
	log_div.setAttribute("class", "log")

	if (log_div.children.length > 64) {
		log_div.removeChild(log_div.firstChild)
	}
}

function limpar () {
	log_div.innerHTML = ''
}

function def_titulo (str) {
	document.title = str
}

function ad_img (str) {
	const img = document.createElement("img")
	img.src = str
	document.body.appendChild(img)
}
