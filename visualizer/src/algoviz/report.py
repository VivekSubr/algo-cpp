from __future__ import annotations

import json
import os
import shutil
import subprocess
import webbrowser
from pathlib import Path

from algoviz.models import FunctionDefinition
from algoviz.runtime import RuntimeResult
from algoviz.workspace import RunWorkspace


def _script_json(value: object) -> str:
    return json.dumps(value, separators=(",", ":")).replace("</", "<\\/")


def write_report(
    *,
    workspace: RunWorkspace,
    source_path: Path,
    selected: list[FunctionDefinition],
    test_filter: str,
    result: RuntimeResult,
) -> Path:
    report_path = workspace.report / "index.html"
    payload = {
        "events": result.events,
        "source": source_path.read_text(encoding="utf-8"),
        "sourceName": source_path.name,
        "status": result.status,
        "test": test_filter,
        "function": ", ".join(item.qualified_name for item in selected),
        "stdout": result.stdout,
        "stderr": result.stderr,
    }
    report_path.write_text(_html(_script_json(payload)), encoding="utf-8")
    return report_path


def open_report(path: Path) -> bool:
    if os.environ.get("WSL_DISTRO_NAME") and shutil.which("explorer.exe"):
        converted = subprocess.run(
            ["wslpath", "-w", str(path)],
            capture_output=True,
            text=True,
            check=False,
        )
        if converted.returncode == 0:
            subprocess.Popen(
                ["explorer.exe", converted.stdout.strip()],
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
            )
            return True
    return webbrowser.open(path.resolve().as_uri())


def _html(payload: str) -> str:
  return fr'''<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>AlgoViz trace</title>
<style>
:root {{
  --ink: #17211d;
  --muted: #66736d;
  --paper: #f5f7f5;
  --panel: #ffffff;
  --line: #d8dedb;
  --teal: #087f6d;
  --teal-soft: #dff4ed;
  --amber: #b65f12;
  --amber-soft: #fff0dc;
  --danger: #bd2c3d;
  --danger-soft: #fde7e9;
}}
* {{ box-sizing: border-box; }}
html, body {{ height: 100%; }}
body {{
  margin: 0;
  color: var(--ink);
  background-color: var(--paper);
  background-image: linear-gradient(rgba(23, 33, 29, .035) 1px, transparent 1px),
                    linear-gradient(90deg, rgba(23, 33, 29, .035) 1px, transparent 1px);
  background-size: 24px 24px;
  font-family: "Aptos", "Segoe UI", sans-serif;
}}
button, input {{ font: inherit; }}
.app {{ min-height: 100%; display: grid; grid-template-rows: auto 1fr auto; }}
.topbar {{
  min-height: 68px;
  padding: 12px 20px;
  border-bottom: 1px solid var(--line);
  background: rgba(255, 255, 255, .94);
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 18px;
}}
.brand {{ display: flex; align-items: baseline; gap: 14px; min-width: 0; }}
.brand h1 {{ margin: 0; font: 700 22px Georgia, serif; letter-spacing: 0; }}
.subject {{ color: var(--muted); overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }}
.status {{
  padding: 5px 9px;
  border: 1px solid var(--line);
  border-radius: 4px;
  font-size: 12px;
  font-weight: 700;
  text-transform: uppercase;
}}
.status.passed {{ color: var(--teal); background: var(--teal-soft); border-color: #9fd8c7; }}
.status.failed, .status.timed_out {{ color: var(--danger); background: var(--danger-soft); border-color: #efb7be; }}
.workspace {{ min-height: 0; display: grid; grid-template-columns: minmax(420px, 1.15fr) minmax(360px, .85fr); }}
.source-panel, .inspect-panel {{ min-height: 0; overflow: auto; background: var(--panel); }}
.source-panel {{ border-right: 1px solid var(--line); }}
.inspect-panel {{
  position: sticky;
  top: 16px;
  align-self: start;
  max-height: calc(100vh - 32px);
}}
.panel-heading {{
  position: sticky;
  top: 0;
  z-index: 3;
  height: 43px;
  padding: 0 16px;
  border-bottom: 1px solid var(--line);
  background: rgba(255, 255, 255, .96);
  display: flex;
  align-items: center;
  justify-content: space-between;
  font-size: 13px;
  font-weight: 700;
}}
.function-label {{ color: var(--teal); font-family: "Cascadia Code", Consolas, monospace; font-size: 12px; }}
.code {{ margin: 0; padding: 10px 0 28px; font: 13px/1.65 "Cascadia Code", Consolas, monospace; }}
.code-line {{ display: grid; grid-template-columns: 54px 1fr; min-height: 22px; padding-right: 16px; white-space: pre; }}
.code-line.active {{ background: var(--amber-soft); box-shadow: inset 3px 0 var(--amber); }}
.line-number {{ color: #98a39e; text-align: right; padding-right: 14px; user-select: none; }}
.inspect-body {{ padding: 16px; }}
.event-meta {{ display: flex; gap: 8px; align-items: center; margin-bottom: 15px; color: var(--muted); font-size: 13px; }}
.event-meta strong {{ color: var(--ink); font-family: "Cascadia Code", Consolas, monospace; }}
.variables {{ display: grid; gap: 12px; }}
.variable {{ border: 1px solid var(--line); border-radius: 6px; background: #fff; overflow: hidden; }}
.variable-header {{ padding: 8px 10px; border-bottom: 1px solid var(--line); display: flex; justify-content: space-between; gap: 10px; }}
.variable-name {{ color: var(--teal); font: 700 12px "Cascadia Code", Consolas, monospace; }}
.variable-value {{ padding: 10px; font: 13px "Cascadia Code", Consolas, monospace; overflow-wrap: anywhere; }}
.object-graph {{ border-color: #9eb4aa; }}
.graph-scroll {{ min-height: 230px; overflow-x: auto; overflow-y: hidden; background: #f9fbfa; }}
.graph-canvas {{ display: block; min-height: 230px; }}
.graph-edge {{ fill: none; stroke: var(--teal); stroke-width: 2; }}
.graph-edge.cycle-edge {{ stroke: var(--danger); stroke-width: 2.5; }}
.graph-edge.stale-edge {{ stroke: #9aa6a0; stroke-dasharray: 5 4; }}
.graph-reference {{ fill: var(--teal); font: 700 11px "Cascadia Code", Consolas, monospace; text-anchor: middle; }}
.graph-reference-line {{ stroke: var(--teal); stroke-width: 1.5; }}
.graph-node rect {{ fill: var(--teal-soft); stroke: var(--teal); stroke-width: 2; }}
.graph-node .graph-value {{ fill: var(--ink); font: 700 20px Georgia, serif; text-anchor: middle; }}
.graph-node .graph-address {{ fill: var(--muted); font: 9px "Cascadia Code", Consolas, monospace; text-anchor: middle; }}
.graph-node.stale-node {{ opacity: .5; }}
.graph-node.stale-node rect {{ stroke-dasharray: 5 4; }}
.graph-null {{ fill: var(--muted); font: 10px "Cascadia Code", Consolas, monospace; text-anchor: middle; }}
.list-track {{ min-height: 90px; padding: 13px 10px; display: flex; align-items: center; gap: 7px; overflow-x: auto; }}
.list-node {{ flex: 0 0 76px; border: 2px solid var(--teal); border-radius: 6px; background: var(--teal-soft); text-align: center; }}
.node-value {{ padding: 8px 5px 5px; font: 700 18px Georgia, serif; }}
.node-address {{ padding: 4px; border-top: 1px solid #9fd8c7; color: var(--muted); font: 9px "Cascadia Code", Consolas, monospace; }}
.arrow {{ color: var(--muted); font-size: 19px; }}
.cycle {{ margin: 0 10px 10px; padding: 6px 8px; color: var(--danger); background: var(--danger-soft); border-left: 3px solid var(--danger); font-size: 12px; font-weight: 700; }}
.empty {{ padding: 36px 12px; color: var(--muted); text-align: center; }}
.console {{ margin-top: 16px; }}
.console summary {{ cursor: pointer; color: var(--muted); font-size: 12px; font-weight: 700; }}
.console pre {{ max-height: 180px; overflow: auto; padding: 10px; background: #17211d; color: #edf4f1; border-radius: 6px; font: 11px/1.5 "Cascadia Code", Consolas, monospace; white-space: pre-wrap; }}
.timeline {{
  min-height: 74px;
  padding: 10px 20px;
  border-top: 1px solid var(--line);
  background: #fff;
  display: grid;
  grid-template-columns: auto minmax(120px, 1fr) auto;
  align-items: center;
  gap: 16px;
}}
.controls {{ display: flex; gap: 5px; }}
.icon-button {{
  width: 36px;
  height: 36px;
  border: 1px solid var(--line);
  border-radius: 4px;
  background: #fff;
  color: var(--ink);
  cursor: pointer;
}}
.icon-button:hover {{ border-color: var(--teal); color: var(--teal); }}
.icon-button.primary {{ background: var(--teal); border-color: var(--teal); color: #fff; }}
input[type="range"] {{ width: 100%; accent-color: var(--teal); }}
.step-count {{ min-width: 94px; color: var(--muted); font: 12px "Cascadia Code", Consolas, monospace; text-align: right; }}
@media (max-width: 780px) {{
  .topbar {{ align-items: flex-start; }}
  .brand {{ display: block; }}
  .subject {{ margin-top: 4px; max-width: 70vw; }}
  .workspace {{ grid-template-columns: 1fr; grid-template-rows: minmax(320px, 52vh) auto; }}
  .source-panel {{ border-right: 0; border-bottom: 1px solid var(--line); }}
  .inspect-panel {{ position: static; max-height: none; }}
  .timeline {{ grid-template-columns: auto 1fr; }}
  .step-count {{ grid-column: 1 / -1; text-align: center; }}
}}
</style>
</head>
<body>
<div class="app">
  <header class="topbar">
    <div class="brand"><h1>AlgoViz</h1><div class="subject" id="subject"></div></div>
    <div class="status" id="status"></div>
  </header>
  <main class="workspace">
    <section class="source-panel" id="sourcePanel">
      <div class="panel-heading"><span id="sourceName"></span><span class="function-label" id="functionName"></span></div>
      <pre class="code" id="code"></pre>
    </section>
    <section class="inspect-panel">
      <div class="panel-heading"><span>Runtime state</span><span id="lineLabel"></span></div>
      <div class="inspect-body">
        <div class="event-meta" id="eventMeta"></div>
        <div class="variables" id="variables"></div>
        <details class="console"><summary>Test output</summary><pre id="console"></pre></details>
      </div>
    </section>
  </main>
  <footer class="timeline">
    <div class="controls">
      <button class="icon-button" id="previous" title="Previous step" aria-label="Previous step">&#9664;</button>
      <button class="icon-button primary" id="play" title="Play trace" aria-label="Play trace">&#9654;</button>
      <button class="icon-button" id="next" title="Next step" aria-label="Next step">&#9654;</button>
    </div>
    <input id="scrubber" type="range" min="0" value="0" aria-label="Trace step">
    <div class="step-count" id="stepCount"></div>
  </footer>
</div>
<script>
const data = {payload};
const events = data.events;
let current = 0;
let timer = null;
const code = document.getElementById('code');
const variables = document.getElementById('variables');
const scrubber = document.getElementById('scrubber');
const play = document.getElementById('play');

document.getElementById('subject').textContent = data.test;
document.getElementById('sourceName').textContent = data.sourceName;
document.getElementById('functionName').textContent = data.function;
const status = document.getElementById('status');
status.textContent = data.status.replace('_', ' ');
status.classList.add(data.status);
document.getElementById('console').textContent = [data.stdout, data.stderr].filter(Boolean).join('\n');

data.source.split('\n').forEach((text, index) => {{
  const row = document.createElement('span');
  row.className = 'code-line';
  row.dataset.line = String(index + 1);
  const number = document.createElement('span');
  number.className = 'line-number';
  number.textContent = String(index + 1);
  const content = document.createElement('span');
  content.textContent = text || ' ';
  row.append(number, content);
  code.appendChild(row);
}});

function shortAddress(address) {{
  if (!address) return 'null';
  return address.length > 9 ? '...' + address.slice(-6) : address;
}}

function svgElement(name, attributes = {{}}) {{
  const element = document.createElementNS('http://www.w3.org/2000/svg', name);
  Object.entries(attributes).forEach(([key, value]) => element.setAttribute(key, String(value)));
  return element;
}}

function objectGraphAt(eventIndex) {{
  const nodes = new Map();
  for (let index = 0; index <= eventIndex; ++index) {{
    Object.values(events[index].variables).forEach(value => {{
      if (!value || value.kind !== 'linked-list') return;
      value.nodes.forEach(node => nodes.set(node.id, {{...node, lastSeen: index}}));
    }});
  }}
  const references = new Map();
  Object.entries(events[eventIndex].variables).forEach(([name, value]) => {{
    if (!value || value.kind !== 'linked-list' || !value.head) return;
    const names = references.get(value.head) || [];
    names.push(name);
    references.set(value.head, names);
  }});
  return {{nodes, references}};
}}

function isCycleEdge(source, target, nodes) {{
  const visited = new Set();
  let currentId = target;
  while (currentId && nodes.has(currentId) && !visited.has(currentId)) {{
    if (currentId === source) return true;
    visited.add(currentId);
    currentId = nodes.get(currentId).next;
  }}
  return false;
}}

function renderObjectGraph(eventIndex) {{
  const graph = objectGraphAt(eventIndex);
  if (!graph.nodes.size) return null;

  const wrapper = document.createElement('article');
  wrapper.className = 'variable object-graph';
  const header = document.createElement('div');
  header.className = 'variable-header';
  header.innerHTML = `<span class="variable-name">All nodes</span><span>${{graph.nodes.size}} known</span>`;
  wrapper.appendChild(header);

  const nodeEntries = Array.from(graph.nodes.entries());
  const nodeIndex = new Map(nodeEntries.map(([id], index) => [id, index]));
  const width = Math.max(520, 70 + nodeEntries.length * 132);
  const height = 230;
  const centerY = 115;
  const scroll = document.createElement('div');
  scroll.className = 'graph-scroll';
  const canvas = svgElement('svg', {{
    class: 'graph-canvas',
    viewBox: `0 0 ${{width}} ${{height}}`,
    width,
    height,
    role: 'img',
    'aria-label': 'All linked-list nodes and connections',
  }});

  const definitions = svgElement('defs');
  const marker = svgElement('marker', {{
    id: 'graph-arrow',
    viewBox: '0 0 10 10',
    refX: 9,
    refY: 5,
    markerWidth: 6,
    markerHeight: 6,
    orient: 'auto-start-reverse',
  }});
  marker.appendChild(svgElement('path', {{d: 'M 0 0 L 10 5 L 0 10 z', fill: 'context-stroke'}}));
  definitions.appendChild(marker);
  canvas.appendChild(definitions);

  nodeEntries.forEach(([id, node], index) => {{
    const sourceX = 70 + index * 132;
    const stale = node.lastSeen !== eventIndex;
    if (node.next && nodeIndex.has(node.next)) {{
      const targetIndex = nodeIndex.get(node.next);
      const targetX = 70 + targetIndex * 132;
      let pathData;
      if (targetIndex === index + 1) {{
        pathData = `M ${{sourceX + 40}} ${{centerY}} L ${{targetX - 40}} ${{centerY}}`;
      }} else if (targetIndex === index) {{
        pathData = `M ${{sourceX + 26}} ${{centerY + 32}} C ${{sourceX + 70}} 205 ${{sourceX - 70}} 205 ${{sourceX - 26}} ${{centerY + 32}}`;
      }} else {{
        const edgeY = 198 + Math.min(18, Math.abs(targetIndex - index) * 3);
        pathData = `M ${{sourceX}} ${{centerY + 32}} C ${{sourceX}} ${{edgeY}} ${{targetX}} ${{edgeY}} ${{targetX}} ${{centerY + 32}}`;
      }}
      const edgeClasses = ['graph-edge'];
      if (isCycleEdge(id, node.next, graph.nodes)) edgeClasses.push('cycle-edge');
      if (stale) edgeClasses.push('stale-edge');
      const edge = svgElement('path', {{
        d: pathData,
        class: edgeClasses.join(' '),
        'marker-end': 'url(#graph-arrow)',
      }});
      const title = svgElement('title');
      title.textContent = `${{node.value}} next -> ${{graph.nodes.get(node.next).value}}`;
      edge.appendChild(title);
      canvas.appendChild(edge);
    }} else {{
      canvas.appendChild(svgElement('path', {{
        d: `M ${{sourceX}} ${{centerY + 32}} L ${{sourceX}} ${{centerY + 56}}`,
        class: `graph-edge${{stale ? ' stale-edge' : ''}}`,
        'marker-end': 'url(#graph-arrow)',
      }}));
      const nullLabel = svgElement('text', {{x: sourceX, y: centerY + 77, class: 'graph-null'}});
      nullLabel.textContent = 'null';
      canvas.appendChild(nullLabel);
    }}
  }});

  nodeEntries.forEach(([id, node], index) => {{
    const x = 70 + index * 132;
    const references = graph.references.get(id);
    if (references) {{
      const label = svgElement('text', {{x, y: 24, class: 'graph-reference'}});
      label.textContent = references.join(', ');
      canvas.appendChild(label);
      canvas.appendChild(svgElement('line', {{
        x1: x,
        y1: 31,
        x2: x,
        y2: centerY - 35,
        class: 'graph-reference-line',
        'marker-end': 'url(#graph-arrow)',
      }}));
    }}

    const group = svgElement('g', {{
      class: `graph-node${{node.lastSeen === eventIndex ? '' : ' stale-node'}}`,
      transform: `translate(${{x}} ${{centerY}})`,
    }});
    group.appendChild(svgElement('rect', {{x: -40, y: -32, width: 80, height: 64, rx: 5}}));
    const value = svgElement('text', {{x: 0, y: -7, class: 'graph-value'}});
    value.textContent = String(node.value);
    group.appendChild(value);
    const address = svgElement('text', {{x: 0, y: 18, class: 'graph-address'}});
    address.textContent = shortAddress(id);
    group.appendChild(address);
    const title = svgElement('title');
    title.textContent = node.lastSeen === eventIndex ? id : `${{id}} (last observed)`;
    group.appendChild(title);
    canvas.appendChild(group);
  }});

  scroll.appendChild(canvas);
  wrapper.appendChild(scroll);
  if (nodeEntries.some(([id, node]) => node.next && isCycleEdge(id, node.next, graph.nodes))) {{
    const warning = document.createElement('div');
    warning.className = 'cycle';
    warning.textContent = 'Cycle detected';
    wrapper.appendChild(warning);
  }}
  return wrapper;
}}

function renderList(name, value) {{
  const wrapper = document.createElement('article');
  wrapper.className = 'variable';
  const header = document.createElement('div');
  header.className = 'variable-header';
  header.innerHTML = `<span class="variable-name"></span><span>${{value.nodes.length}} node${{value.nodes.length === 1 ? '' : 's'}}</span>`;
  header.querySelector('.variable-name').textContent = name;
  wrapper.appendChild(header);
  const track = document.createElement('div');
  track.className = 'list-track';
  if (!value.nodes.length) {{
    track.textContent = 'null';
  }} else {{
    value.nodes.forEach((node, index) => {{
      const box = document.createElement('div');
      box.className = 'list-node';
      const nodeValue = document.createElement('div');
      nodeValue.className = 'node-value';
      nodeValue.textContent = String(node.value);
      const address = document.createElement('div');
      address.className = 'node-address';
      address.textContent = shortAddress(node.id);
      box.append(nodeValue, address);
      track.appendChild(box);
      if (node.next) {{
        const arrow = document.createElement('span');
        arrow.className = 'arrow';
        arrow.textContent = index === value.nodes.length - 1 && value.cycle ? '\u21a9' : '\u2192';
        track.appendChild(arrow);
      }}
    }});
  }}
  wrapper.appendChild(track);
  if (value.cycle) {{
    const warning = document.createElement('div');
    warning.className = 'cycle';
    warning.textContent = 'Cycle detected';
    wrapper.appendChild(warning);
  }}
  return wrapper;
}}

function renderScalar(name, value) {{
  const wrapper = document.createElement('article');
  wrapper.className = 'variable';
  const header = document.createElement('div');
  header.className = 'variable-header';
  const label = document.createElement('span');
  label.className = 'variable-name';
  label.textContent = name;
  header.appendChild(label);
  const content = document.createElement('div');
  content.className = 'variable-value';
  content.textContent = typeof value === 'string' ? value : JSON.stringify(value);
  wrapper.append(header, content);
  return wrapper;
}}

function render() {{
  document.querySelectorAll('.code-line.active').forEach(row => row.classList.remove('active'));
  variables.replaceChildren();
  if (!events.length) {{
    variables.innerHTML = '<div class="empty">No trace events were captured.</div>';
    document.getElementById('eventMeta').textContent = 'The scenario did not reach the selected source.';
    document.getElementById('lineLabel').textContent = '';
    document.getElementById('stepCount').textContent = '0 / 0';
    return;
  }}
  const event = events[current];
  const activeLine = document.querySelector(`.code-line[data-line="${{event.line}}"]`);
  if (activeLine) {{
    activeLine.classList.add('active');
  }}
  document.getElementById('eventMeta').innerHTML = `In <strong></strong>`;
  document.getElementById('eventMeta').querySelector('strong').textContent = event.function;
  document.getElementById('lineLabel').textContent = `Line ${{event.line}}`;
  const objectGraph = renderObjectGraph(current);
  if (objectGraph) variables.appendChild(objectGraph);
  Object.entries(event.variables).forEach(([name, value]) => {{
    variables.appendChild(value && value.kind === 'linked-list' ? renderList(name, value) : renderScalar(name, value));
  }});
  scrubber.value = String(current);
  document.getElementById('stepCount').textContent = `${{current + 1}} / ${{events.length}}`;
}}

function stop() {{
  if (timer) window.clearInterval(timer);
  timer = null;
  play.innerHTML = '&#9654;';
  play.title = 'Play trace';
}}

function setStep(next) {{
  current = Math.max(0, Math.min(events.length - 1, next));
  render();
}}

document.getElementById('previous').addEventListener('click', () => {{ stop(); setStep(current - 1); }});
document.getElementById('next').addEventListener('click', () => {{ stop(); setStep(current + 1); }});
play.addEventListener('click', () => {{
  if (timer) {{ stop(); return; }}
  play.innerHTML = '&#10074;&#10074;';
  play.title = 'Pause trace';
  timer = window.setInterval(() => {{
    if (current >= events.length - 1) {{ stop(); return; }}
    setStep(current + 1);
  }}, 550);
}});
scrubber.max = String(Math.max(0, events.length - 1));
scrubber.addEventListener('input', event => {{ stop(); setStep(Number(event.target.value)); }});
document.addEventListener('keydown', event => {{
  if (event.key === 'ArrowLeft') {{ stop(); setStep(current - 1); }}
  if (event.key === 'ArrowRight') {{ stop(); setStep(current + 1); }}
}});
render();
</script>
</body>
</html>
'''