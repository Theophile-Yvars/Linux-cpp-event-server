# Linux-cpp-event-server

## Schéma 

                ┌─────────────┐
                │ Thread I/O  │
                └──────┬──────┘
                       │ push(event)
                       ▼
                ┌────────────────┐
                │ ThreadSafeQueue│
                └──────┬─────────┘
                       │ pop()
        ┌──────────────┼──────────────┐
        ▼              ▼              ▼
   Worker 1        Worker 2        Worker N
        │              │              │
        ▼              ▼              ▼
   ruleEngine     ruleEngine     ruleEngine
        │              │              │
        ▼              ▼              ▼
      action         action         action





